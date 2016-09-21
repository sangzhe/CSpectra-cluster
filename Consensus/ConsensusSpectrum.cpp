//
// Created by SangZhe on 16/8/31.
//

#include "ConsensusSpectrum.h"
#include "../Spectrum/Peak.h"
#include "../Spectrum/Spectrum.h"
#include "../util/Defaults.h"
#include "../util/Function/peak/BinnedHighestNPeakFunction.h"


int ConsensusSpectrum::DEFAULT_PEAKS_TO_KEEP = 5;

int ConsensusSpectrum::SIZE_TO_ADD_EVERY_TIME = 100;

float ConsensusSpectrum::NOISE_FILTER_INCREMENT = 100;

float ConsensusSpectrum::FRACTION_OF_LOWEST_PEAK_TOKEEP = 0.40F;


const string methodName = "ConsensusSpectrum";
const string methodVersion = "0.1";

float ConsensusSpectrum::FINAL_MZ_THRESHOLD = 0.4F;
float ConsensusSpectrum::MZ_THRESHOLD_STEP = 0.1F;
bool ConsensusSpectrum::USE_ROUNDING = true;
int ConsensusSpectrum::MZ_PRECISION = 1000;

ConsensusSpectrumBuilderFactory* ConsensusSpectrum::FACTORY = new ConsensusSpectrumFactory();


ConsensusSpectrumBuilderFactory* ConsensusSpectrum::buildFactory() {
    return new ConsensusSpectrumFactory();
}

ConsensusSpectrum::ConsensusSpectrum() {};

ConsensusSpectrum::ConsensusSpectrum(string &id) {
    this->id = id;
}

ConsensusSpectrum::ConsensusSpectrum(string &id, const int &nSpectra, const float &sumPrecusorMz, const float &sumPrecursorIntens,
                                     const int &sumCharge, const list<IPeak*> &peaks)  {
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecusorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;
    consensusPeaks.merge(list<IPeak*>(peaks));
    PointerPool::add(peaks);


    setDirty(true);
}

list<IPeak*> ConsensusSpectrum::getInternalPeaks() const {
    return consensusPeaks;
}

void ConsensusSpectrum::addSpectra(const list<ISpectrum*> &spectra) {
    if(spectra.size() < 1) return;

    list<ISpectrum*> in = spectra;
    list<ISpectrum*>::iterator iter;
    for(iter = in.begin();iter != in.end();iter++){
        addSpectra(*iter);
    }
    setDirty(true);

    for (SpectrumHolderListener* listener : listeners)
        listener->onSpectraAdd(this, in);

}

void ConsensusSpectrum::onSpectraAdd(ISpectrumHolder *holder, list<ISpectrum*>& added) {
    PointerPool::add(added);
    addSpectra(added);
}
void ConsensusSpectrum::addSpectra(const ISpectrum *merged)  {
    list<IPeak*> spectrumPeaks = merged->getPeaks();
    addPeaks(spectrumPeaks);
    sumCharge += merged->getPrecursorCharge();
    sumPrecursorMz += merged->getPrecursorMz();
    sumPrecursorIntens += 0;

    nSpectra++;
}

void ConsensusSpectrum::onSpectraRemove(ISpectrumHolder *holder, list<ISpectrum*>& removed) {
    removeSpectra(removed);
    PointerPool::remove(removed);
}
void ConsensusSpectrum::removeSpectra(const list<ISpectrum*> &spectra) {
    if(spectra.size() < 1) return;

    list<ISpectrum*> in = spectra;
    list<ISpectrum*>::iterator iter;
    for(iter = in.begin();iter != in.end();iter++){
        removeSpectra(*iter);
}
    setDirty(true);

    for (SpectrumHolderListener* listener : listeners)
        listener->onSpectraRemove(this, in);

}

void ConsensusSpectrum::removeSpectra(const ISpectrum *removed) {
    list<IPeak*> spectrumPeaks = removed->getPeaks();
    removePeaks(spectrumPeaks);

    sumCharge -= removed->getPrecursorCharge();
    sumPrecursorMz -= removed->getPrecursorMz();
    sumPrecursorIntens -= 0;

    nSpectra--;
}

void ConsensusSpectrum::removePeaks(const list<IPeak*> &peaksToRemove) {
    vector<IPeak*> ALLpeaks(allPeaks.begin(),allPeaks.end());
    int posAllPeaks = 0;
    list<IPeak*> in = peaksToRemove;
    int original_size = in.size();
    for(list<IPeak*>::iterator iter = in.begin();iter != in.end();iter++){
        IPeak* peakToRemove = *iter;
        double mzToRemove = peakToRemove->getMz();
        if (USE_ROUNDING)
            mzToRemove = MZIntensityUtilities::round(mzToRemove, MZ_PRECISION);
        for(int j = posAllPeaks;j < ALLpeaks.size();j++){
            IPeak* currentExistingPeak = ALLpeaks[j];
            if (mzToRemove < currentExistingPeak->getMz()) {
                posAllPeaks = j;
                break;
            }
            if (mzToRemove == currentExistingPeak->getMz()) {
                IPeak* newPeak = new Peak(currentExistingPeak->getMz(),currentExistingPeak->getIntensity() - peakToRemove->getIntensity(),currentExistingPeak->getCount() - 1);
                ALLpeaks[j] = newPeak;
                PointerPool::add(newPeak);
                PointerPool::remove(currentExistingPeak);
                posAllPeaks = j;
                break;
            }
        }
    }

    list<IPeak*> tmp;
    for (IPeak* p : ALLpeaks) {
        if (p->getCount() > 0)
            tmp.push_back(p);
        else
            PointerPool::remove(p);
    }
    PointerPool::remove(allPeaks);
    allPeaks.clear();
    allPeaks.merge(tmp);
}

void ConsensusSpectrum::addHeldPeaks() {
    list<IPeak*> added;
    unordered_set<IPeak*>::iterator iter;
    for(iter = heldPeaks.begin();iter != heldPeaks.end();iter++){
        added.push_back(*iter);
    }
    added.sort(Peak::cmpPeak);
    internalAddPeaks(added);
    heldPeaks.clear();
}

void ConsensusSpectrum::internalAddPeaks(const list<IPeak*> &peaksToAdd) {
    list<IPeak*> in = peaksToAdd;
    int posAllPeaks = 0;
    list<IPeak*> newPeaks;

    vector<IPeak*> ALLpeaks(in.begin(),in.end());
    for(list<IPeak*>::iterator iter = in.begin();iter != in.end();iter++){
        IPeak* peakToAdd = *iter;
        double mzToAdd = peakToAdd->getMz();

        if (USE_ROUNDING)
            mzToAdd = MZIntensityUtilities::round(mzToAdd, MZ_PRECISION);

        bool wasAdded = false;

    for(int j = posAllPeaks;j < allPeaks.size();j++){
        IPeak* currentExistingPeak = ALLpeaks[j];

        if (mzToAdd < currentExistingPeak->getMz()) {
            IPeak* newPeak = new Peak((float) mzToAdd, peakToAdd->getIntensity(), peakToAdd->getCount());
            newPeaks.push_back(newPeak);
            PointerPool::add(newPeak);
            posAllPeaks = j;
            wasAdded = true;
            break;
        }

        if (mzToAdd == currentExistingPeak->getMz()) {
            IPeak* newPeak = new Peak(
                    currentExistingPeak->getMz(),
                    peakToAdd->getIntensity() + currentExistingPeak->getIntensity(),
                    currentExistingPeak->getCount() + peakToAdd->getCount());
            PointerPool::add(newPeak);

            ALLpeaks[j] = newPeak;
            PointerPool::remove(currentExistingPeak);
            posAllPeaks = j;
            wasAdded = true;
            break;
        }
        if (!wasAdded){
            IPeak* newPeak = new Peak((float) mzToAdd, peakToAdd->getIntensity(), peakToAdd->getCount());
            PointerPool::add(newPeak);
            newPeaks.push_back(newPeak);
        }
    }
    }

    allPeaks.merge(newPeaks);
    allPeaks.sort(Peak::cmpPeakMz);
}

list<IPeak*> ConsensusSpectrum::findConsensusPeaks(const list<IPeak*> &input, const int &peaksToKeep, int &nSpectra){
    // Step 1: merge identical peaks
    list<IPeak*> ret = mergeIdenticalPeaks(input);
    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    ret = adaptPeakIntensities(ret,nSpectra);
    // Step 3: filter the spectrum
    ret = filterNoise(ret);
    return ret;
}

list<IPeak*> ConsensusSpectrum::mergeIdenticalPeaks( const list<IPeak*> &inPeaks) {
    list<IPeak*> filterdPeaks;
    if(inPeaks.size() == 0) return filterdPeaks;

    filterdPeaks.merge(list<IPeak*>(inPeaks));

    for(float range = MZ_THRESHOLD_STEP;range <= FINAL_MZ_THRESHOLD;range += MZ_THRESHOLD_STEP){
        list<IPeak*> newPeakList;
        IPeak* currentPeak = *filterdPeaks.begin();

        list<IPeak*>::iterator iter = filterdPeaks.begin();
        iter++;
        for(iter;iter != filterdPeaks.end(); iter++){
            IPeak* nextPeak = *iter;

            float nextPeakMz = nextPeak->getMz();
            float currentPeakMz = currentPeak->getMz();
            float testLimit = currentPeakMz +range;

            if(nextPeakMz <= testLimit){
                double nextPeakIntensity = nextPeak->getIntensity();
                double currentPeakIntensity = currentPeak->getIntensity();
                double totalIntensity = nextPeakIntensity + currentPeakIntensity;
                double nextPeakFraction = nextPeakIntensity /totalIntensity;
                double currentPeakFraction = currentPeakIntensity / totalIntensity;

                double weightedMz = (nextPeakFraction * nextPeakMz) + (currentPeakFraction * currentPeakMz);

                double intensity = currentPeakIntensity + nextPeakIntensity;

                int count = currentPeak->getCount() + nextPeak->getCount();

                IPeak *newPeak  = new Peak((float)weightedMz,(float)intensity,count);

                currentPeak = newPeak;
            }else{
                newPeakList.push_back(currentPeak);
                currentPeak = nextPeak;

            }
        }
        newPeakList.push_back(currentPeak);
        PointerPool::remove(inPeaks);
        filterdPeaks.clear();
        filterdPeaks.merge(list<IPeak*>(newPeakList));
        PointerPool::add(newPeakList);
    }
    return filterdPeaks;
}

list<IPeak*> ConsensusSpectrum::adaptPeakIntensities(const list<IPeak*> &inp, int nSpectra) {
//    int originalCount;
//    ToDo add PeakUtilities class
//    originalCount = PeakUtilities.getTotalCount(inp);
    list<IPeak*> ret = inp;
    list<IPeak*>::iterator iter;
    for(iter = ret.begin();iter != ret.end();iter++){
        IPeak *peak = *iter;
        float peakProbability = (float)peak->getCount() / (float)nSpectra;
        float newIntensity = (float)(peak->getIntensity() * (0.95 +0.05 * pow(1+peakProbability,5)));

        ret.erase(iter);
        PointerPool::remove(peak);

        IPeak* newPeak = new Peak(peak->getMz(),newIntensity,peak->getCount());
        PointerPool::add(newPeak);

        ret.push_back(newPeak);
    }
//    int finalCount;
//    ToDo PeakUtilities;
//    finalCount = PeakUtilities.getTotalCount(ret)
//    if(originalCount != finalCount)
//        throw("Peak merge changed total count");
    return ret;
}

list<IPeak*> ConsensusSpectrum::filterNoise(const list<IPeak*> &inp) {
    list<IPeak*> in = inp;
    in.sort(Peak::cmpPeakMz);
    BinnedHighestNPeakFunction func = BinnedHighestNPeakFunction(DEFAULT_PEAKS_TO_KEEP,(int)NOISE_FILTER_INCREMENT,0);
    in = func.apply(in);
    PointerPool::remove(inp);
    PointerPool::add(in);

//    filteredSpectrum = noiseFilter(inp);
    return in;
}

void ConsensusSpectrum::clear() {
    sumCharge = 0;
    sumPrecursorMz = 0;
    sumPrecursorIntens = 0;
    nSpectra = 0;
    allPeaks.clear();
    consensusPeaks.clear();
    setDirty(true);
}

void ConsensusSpectrum::addPeaks(const list<IPeak*> &peaksToAdd) {
    if(nSpectra < SIZE_TO_ADD_EVERY_TIME){
        internalAddPeaks(peaksToAdd);
        return;
    }
    storeHeldPeaks(peaksToAdd);
    if(nSpectra < (10 * SIZE_TO_ADD_EVERY_TIME)){
        if (nSpectra % SIZE_TO_ADD_EVERY_TIME ==0){
            addHeldPeaks();
        }
    }else{
        if(nSpectra % (4 * SIZE_TO_ADD_EVERY_TIME) == 0){
            addHeldPeaks();
        }
    }
}

void ConsensusSpectrum::storeHeldPeaks(const list<IPeak*> &peaksToAdd) {
    if(nSpectra < SIZE_TO_ADD_EVERY_TIME) throw("cannot add without a fairly large cluster");

    if(lowestConcensusPeak == 0)  getConsensusSpectrum();

    list<IPeak*> in = peaksToAdd;
    float minimuxKeptPeak = lowestConcensusPeak * FRACTION_OF_LOWEST_PEAK_TOKEEP;

    list<IPeak*>::iterator iter;
    for(iter = in.begin();iter != peaksToAdd.end();iter++){
        if((*iter)->getIntensity() > minimuxKeptPeak){
            heldPeaks.insert(*iter);
            PointerPool::add(*iter);
        }
    }

}

void ConsensusSpectrum::update() {
    if (nSpectra > 0) {
        averagePrecursorMz = sumPrecursorMz / nSpectra;
        averageCharge = sumCharge / nSpectra;
        averagePrecursorIntens = sumPrecursorIntens / nSpectra;
    } else {
        averagePrecursorMz = 0;
        averageCharge = 0;
        averagePrecursorIntens = 0;
    }
    if(allPeaks.size() < 1){
        list<IPeak*> emptylist;
        if(consensusSpectrum != nullptr){
            delete consensusSpectrum;
            consensusSpectrum = nullptr;
        }
        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),emptylist);
        setDirty(false);
        return;
    }
    list<IPeak*> newPeaks = findConsensusPeaks(allPeaks,DEFAULT_PEAKS_TO_KEEP,nSpectra);

    consensusPeaks.clear();
    consensusPeaks.merge(list<IPeak*>(newPeaks));
    float minimumConsensusPeak = 0x1.fffffeP+127f;
    list<IPeak*>::iterator iter;
    for(iter = consensusPeaks.begin();iter != consensusPeaks.end();iter++){
        float intensity = (*iter)->getIntensity();
        if( intensity < minimumConsensusPeak && intensity > 0) minimumConsensusPeak = intensity;
        lowestConcensusPeak = minimumConsensusPeak;

        if(consensusSpectrum != nullptr){
            delete consensusSpectrum;
            consensusSpectrum = nullptr;
        }
        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),consensusPeaks);
        setDirty(false);
    }

}

void ConsensusSpectrum::addSpectrumHolderListener(SpectrumHolderListener *added) {
    listeners.push_back(added);
}

void ConsensusSpectrum::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(listeners.begin(),listeners.end(),removed));
    if(iter != listeners.end()){
        listeners.erase(iter);
    }
}

ISpectrum* ConsensusSpectrum::getConsensusSpectrum() {
    if(isDirty) update();
    return internalGetConcensusSpectrum();
}
ISpectrum* ConsensusSpectrum::internalGetConcensusSpectrum() const{
    return consensusSpectrum;
}

int ConsensusSpectrum::getSumCharge() const{
    return sumCharge;
}

double ConsensusSpectrum::getSumPrecursorMz()const {
    return sumPrecursorMz;
}

double ConsensusSpectrum::getSumPrecursorIntensity() const{
    return sumPrecursorIntens;
}

int ConsensusSpectrum::getSpectraCount()const {
    return nSpectra;
}

bool ConsensusSpectrum::isRemovedSupported() {
    return true;
}

void ConsensusSpectrum::setDirty(bool isDirty){
    this->isDirty = isDirty;
}
string ConsensusSpectrum::getMethodName() {
    return methodName;
}


ConsensusSpectrum::~ConsensusSpectrum() {
    delete consensusSpectrum,FACTORY;
    consensusSpectrum,FACTORY= nullptr;
    PointerPool::remove(allPeaks);
    PointerPool::remove(consensusPeaks);
    for(IPeak* p:heldPeaks){
        PointerPool::remove(p);
    }
}


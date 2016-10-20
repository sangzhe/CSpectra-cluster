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

PointerPool* ConsensusSpectrum::pointer_pool = PoolFactory::getInstance();

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
                                     const int &sumCharge, const vector<Peak> &peaks)  {
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecusorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;

    for(const Peak& peak:peaks){
        consensusPeaks.push_back(peak);
    }


    setDirty(true);
}

vector<Peak> ConsensusSpectrum::getInternalPeaks() const {
    return consensusPeaks;
}

void ConsensusSpectrum::addSpectra( vector<ISpectrum*> &spectra) {

    if(spectra.size() < 1) return;

    for(ISpectrum* spectrum:spectra){
        addSpectra(spectrum);
    }
    setDirty(true);

    for (SpectrumHolderListener* listener : listeners)
        listener->onSpectraAdd(this, spectra);

}

void ConsensusSpectrum::onSpectraAdd(ISpectrumHolder *holder, vector<ISpectrum*>& added) {
//      not necessary
//    pointer_pool->add(added);
    addSpectra(added);
}
void ConsensusSpectrum::addSpectra( ISpectrum *merged)  {
    vector<Peak> spectrumPeaks = merged->getPeaks();
    addPeaks(spectrumPeaks);
    sumCharge += merged->getPrecursorCharge();
    sumPrecursorMz += merged->getPrecursorMz();
    sumPrecursorIntens += 0;

    nSpectra++;
}

void ConsensusSpectrum::onSpectraRemove(ISpectrumHolder *holder, vector<ISpectrum*>& removed) {
    removeSpectra(removed);
//    pointer_pool->remove(removed);
}
void ConsensusSpectrum::removeSpectra( vector<ISpectrum*> &spectra) {
    if(spectra.size() < 1) return;

    for(ISpectrum* spectrum:spectra){
        removeSpectra(spectrum);
}
    setDirty(true);

    for (SpectrumHolderListener* listener : listeners)
        listener->onSpectraRemove(this, spectra);

}

void ConsensusSpectrum::removeSpectra( ISpectrum *removed) {
    vector<Peak> spectrumPeaks = removed->getPeaks();
    removePeaks(spectrumPeaks);

    sumCharge -= removed->getPrecursorCharge();
    sumPrecursorMz -= removed->getPrecursorMz();
    sumPrecursorIntens -= 0;

    nSpectra--;
}

void ConsensusSpectrum::removePeaks(const vector<Peak> &peaksToRemove) {
    int posAllPeaks = 0;
    vector<Peak> in = peaksToRemove;
    int original_size = in.size();
    for(Peak& peak:in){
        double mzToRemove = peak.getMz();
        if (USE_ROUNDING)
            mzToRemove = MZIntensityUtilities::round(mzToRemove, MZ_PRECISION);
        for(int j = posAllPeaks;j < allPeaks.size();j++){
            Peak currentExistingPeak = allPeaks[j];
            if (mzToRemove < currentExistingPeak.getMz()) {
                posAllPeaks = j;
                break;
            }
            if (mzToRemove == currentExistingPeak.getMz()) {
                Peak newPeak(currentExistingPeak.getMz(),currentExistingPeak.getIntensity() - peak.getIntensity(),currentExistingPeak.getCount() - 1);
                allPeaks[j] = newPeak;
                posAllPeaks = j;
                break;
            }
        }
    }

    vector<Peak> tmp;
    for (Peak p : allPeaks) {
        if (p.getCount() > 0)
            tmp.push_back(p);
    }
    allPeaks.clear();
    for(Peak& peak:tmp){
        allPeaks.push_back(peak);
    }
}

void ConsensusSpectrum::addHeldPeaks() {
    vector<Peak> added;
    unordered_set<Peak>::iterator iter;
    for(iter = heldPeaks.begin();iter != heldPeaks.end();++iter){
        added.push_back(*iter);
    }
    sort(added.begin(),added.end(),Peak::cmpPeak);
    internalAddPeaks(added);
    heldPeaks.clear();
}

void ConsensusSpectrum::internalAddPeaks(const vector<Peak> &peaksToAdd) {
    vector<Peak> in = peaksToAdd;
    int posAllPeaks = 0;
    vector<Peak> newPeaks;
    for(Peak& peak:in) {
        double mzToAdd = peak.getMz();

        if (USE_ROUNDING)
            mzToAdd = MZIntensityUtilities::round(mzToAdd, MZ_PRECISION);

        bool wasAdded = false;

        for (int j = posAllPeaks; j < allPeaks.size(); j++) {
            Peak currentExistingPeak = allPeaks[j];

            if (mzToAdd < currentExistingPeak.getMz()) {
                Peak newPeak((float) mzToAdd, peak.getIntensity(), peak.getCount());
                newPeaks.push_back(newPeak);
                posAllPeaks = j;
                wasAdded = true;
                break;
            }

            if (mzToAdd == currentExistingPeak.getMz()) {
                Peak newPeak(
                        currentExistingPeak.getMz(),
                        peak.getIntensity() + currentExistingPeak.getIntensity(),
                        currentExistingPeak.getCount() + peak.getCount());

                allPeaks[j] = newPeak;
                posAllPeaks = j;
                wasAdded = true;
                break;
            }
        }
        if (!wasAdded) {
            Peak newPeak((float) mzToAdd, peak.getIntensity(), peak.getCount());
            newPeaks.push_back(newPeak);
        }
    }
    for(Peak& peak:newPeaks){
        allPeaks.push_back(peak);
    }
    sort(allPeaks.begin(),allPeaks.end(),Peak::cmpPeakMz);
}

vector<Peak> ConsensusSpectrum::findConsensusPeaks(const vector<Peak> &input, const int &peaksToKeep, int &nSpectra){
    // Step 1: merge identical peaks
    vector<Peak> ret = mergeIdenticalPeaks(input);
    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    ret = adaptPeakIntensities(ret,nSpectra);
    // Step 3: filter the spectrum
    ret = filterNoise(ret);
    return ret;
}

vector<Peak> ConsensusSpectrum::mergeIdenticalPeaks( const vector<Peak> &inPeaks) {
    vector<Peak> filterdPeaks;
    if(inPeaks.size() == 0) return filterdPeaks;

    for(const Peak& peak:inPeaks){
        filterdPeaks.push_back(peak);
    }

    for(float range = MZ_THRESHOLD_STEP;range <= FINAL_MZ_THRESHOLD;range += MZ_THRESHOLD_STEP){
        vector<Peak> newPeakList;
        Peak currentPeak = *filterdPeaks.begin();

        vector<Peak>::iterator iter = filterdPeaks.begin();
        ++iter;
        for(iter;iter != filterdPeaks.end(); ++iter){
            Peak nextPeak = *iter;

            float nextPeakMz = nextPeak.getMz();
            float currentPeakMz = currentPeak.getMz();
            float testLimit = currentPeakMz +range;

            if(nextPeakMz <= testLimit){
                double nextPeakIntensity = nextPeak.getIntensity();
                double currentPeakIntensity = currentPeak.getIntensity();
                double totalIntensity = nextPeakIntensity + currentPeakIntensity;
                double nextPeakFraction = nextPeakIntensity /totalIntensity;
                double currentPeakFraction = currentPeakIntensity / totalIntensity;

                double weightedMz = (nextPeakFraction * nextPeakMz) + (currentPeakFraction * currentPeakMz);

                double intensity = currentPeakIntensity + nextPeakIntensity;

                int count = currentPeak.getCount() + nextPeak.getCount();

                Peak newPeak((float)weightedMz,(float)intensity,count);

                currentPeak = newPeak;
            }else{
                newPeakList.push_back(currentPeak);
                currentPeak = nextPeak;

            }
        }
        newPeakList.push_back(currentPeak);
        filterdPeaks.clear();
        for(Peak& peak:newPeakList){
            filterdPeaks.push_back(peak);
        }
    }
    return filterdPeaks;
}

vector<Peak> ConsensusSpectrum::adaptPeakIntensities(const vector<Peak> &inp, int nSpectra) {
//    int originalCount;
//    ToDo add PeakUtilities class
//    originalCount = PeakUtilities.getTotalCount(inp);
    vector<Peak> ret = inp;
    vector<Peak>::iterator iter;
    for(iter = ret.begin();iter != ret.end();++iter){
        Peak peak = *iter;
        float peakProbability = (float)peak.getCount() / (float)nSpectra;
        float newIntensity = (float)(peak.getIntensity() * (0.95 +0.05 * pow(1+peakProbability,5)));

        ret.erase(iter);

        Peak newPeak(peak.getMz(),newIntensity,peak.getCount());

        ret.push_back(newPeak);
    }
//    int finalCount;
//    ToDo PeakUtilities;
//    finalCount = PeakUtilities.getTotalCount(ret)
//    if(originalCount != finalCount)
//        throw("Peak merge changed total count");
    return ret;
}

vector<Peak> ConsensusSpectrum::filterNoise(const vector<Peak> &inp) {
    vector<Peak> in = inp;
    sort(in.begin(),in.end(),Peak::cmpPeakMz);
    BinnedHighestNPeakFunction func = BinnedHighestNPeakFunction(DEFAULT_PEAKS_TO_KEEP,(int)NOISE_FILTER_INCREMENT,0);
    in = func.apply(in);

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

void ConsensusSpectrum::addPeaks(const vector<Peak> &peaksToAdd) {
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

void ConsensusSpectrum::storeHeldPeaks(const vector<Peak> &peaksToAdd) {
    if(nSpectra < SIZE_TO_ADD_EVERY_TIME) throw("cannot add without a fairly large cluster");

    if(lowestConcensusPeak == 0)  getConsensusSpectrum();

    vector<Peak> in = peaksToAdd;
    float minimuxKeptPeak = lowestConcensusPeak * FRACTION_OF_LOWEST_PEAK_TOKEEP;

    vector<Peak>::iterator iter;
    for(iter = in.begin();iter != peaksToAdd.end();++iter){
        if((*iter).getIntensity() > minimuxKeptPeak){
            Peak toAdd = *iter;
            heldPeaks.insert(toAdd);
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
        vector<Peak> emptylist;
        if(consensusSpectrum != nullptr){
            delete consensusSpectrum;
            consensusSpectrum = nullptr;
        }
        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),emptylist);
        setDirty(false);
        return;
    }
    vector<Peak> newPeaks = findConsensusPeaks(allPeaks,DEFAULT_PEAKS_TO_KEEP,nSpectra);

    consensusPeaks.clear();
    for(Peak& peak:newPeaks){
        consensusPeaks.push_back(peak);
    }
    float minimumConsensusPeak = 0x1.fffffeP+127f;
    for(Peak& peak:consensusPeaks){
        float intensity = peak.getIntensity();
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
    pointer_pool->add(added);

}

void ConsensusSpectrum::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(listeners.begin(),listeners.end(),removed));
    if(iter != listeners.end()){
        pointer_pool->remove(*iter);
        listeners.erase(iter);
    }
}

ISpectrum* ConsensusSpectrum::getConsensusSpectrum() {
    if(isDirty()) update();
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

bool ConsensusSpectrum::isDirty() {
    return Dirty;
}
void ConsensusSpectrum::setDirty(bool Dirty){
    this->Dirty = Dirty;
}
string ConsensusSpectrum::getMethodName() {
    return methodName;
}


ConsensusSpectrum::~ConsensusSpectrum() {
    for(SpectrumHolderListener* listener:listeners){
        pointer_pool->remove(listener);
    }
    delete consensusSpectrum,FACTORY;
    consensusSpectrum,FACTORY= nullptr;

}


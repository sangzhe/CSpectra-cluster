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
                                     const int &sumCharge, const list<Peak> &peaks)  {
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecusorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;
    consensusPeaks.merge(list<Peak>(peaks));

    setDirty(true);
}

list<Peak> ConsensusSpectrum::getInternalPeaks() const {
    return consensusPeaks;
}

void ConsensusSpectrum::addSpectra(const list<Spectrum> &spectra) {
    if(spectra.size() < 1) return;

    list<Spectrum> in = spectra;
    list<Spectrum>::iterator iter;
    for(iter = in.begin();iter != in.end();iter++){
        addSpectra(*iter);
    }
    setDirty(true);

}

void ConsensusSpectrum::addSpectra(const Spectrum &merged)  {
    list<Peak> spectrumPeaks = merged.getPeaks();
    addPeaks(spectrumPeaks);
    sumCharge += merged.getPrecursorCharge();
    sumPrecursorMz += merged.getPrecursorMz();
    sumPrecursorIntens += 0;

    nSpectra++;
}

void ConsensusSpectrum::removeSpectra(const list<Spectrum> &spectra) {
    if(spectra.size() < 1) return;

    list<Spectrum> in = spectra;
    list<Spectrum>::iterator iter;
    for(iter = in.begin();iter != in.end();iter++){
        removeSpectra(*iter);
}
    setDirty(true);

}

void ConsensusSpectrum::removeSpectra(const Spectrum &removed) {
    list<Peak> spectrumPeaks = removed.getPeaks();
    removePeaks(spectrumPeaks);

    sumCharge -= removed.getPrecursorCharge();
    sumPrecursorMz -= removed.getPrecursorMz();
    sumPrecursorIntens -= 0;

    nSpectra--;
}

void ConsensusSpectrum::removePeaks(const list<Peak> &peaksToRemove) {
//Todo RemovePeaks;
}

void ConsensusSpectrum::addHeldPeaks() {
    list<Peak> added;
    unordered_set<Peak>::iterator iter;
    for(iter = heldPeaks.begin();iter != heldPeaks.end();iter++){
        added.push_back(*iter);
    }
    added.sort(Peak::cmpPeak);
    internalAddPeaks(added);
    heldPeaks.clear();
}

void ConsensusSpectrum::internalAddPeaks(const list<Peak> &peaksToAdd) {
    list<Peak> peaks = peaksToAdd;
    list<Peak> newPeaks ;

    list<Peak>::iterator iter;

    list<Peak> PeaksToUpdate ;
    for(iter = peaks.begin() ;iter != peaksToAdd.end();iter++){
        Peak peakToAdd = *iter;
        double mzToAdd = peakToAdd.getMz();
        if(USE_ROUNDING){
            mzToAdd = MZIntensityUtilities::round(mzToAdd, MZ_PRECISION);
        }
        bool wasAdded = false;

        list<Peak>::iterator iter1;
        for(iter1 = allPeaks.begin();iter1 != allPeaks.end();){
            Peak currentExistingPeak = *iter1;
            if(mzToAdd < currentExistingPeak.getMz()){
                Peak add((float)mzToAdd,peakToAdd.getIntensity(),peakToAdd.getCount());
                newPeaks.push_back(add);
                wasAdded = true;
                iter1++;
                break;
            }
            if(mzToAdd == currentExistingPeak.getMz()){
                PeaksToUpdate.push_back(*iter);
                Peak add(currentExistingPeak.getMz(),peakToAdd.getIntensity()+currentExistingPeak.getIntensity(),currentExistingPeak.getCount()+peakToAdd.getCount());
                allPeaks.push_back(add);
                wasAdded = true;
                iter1++;
                break;
            }
        }
        if(!wasAdded)
            newPeaks.push_back(Peak((float)mzToAdd,peakToAdd.getIntensity(),peakToAdd.getCount()));

    }
    for(iter = PeaksToUpdate.begin();iter != PeaksToUpdate.end();iter++){
        allPeaks.erase(iter);
    }
    allPeaks.merge(list<Peak>(newPeaks));
    allPeaks.sort(Peak::cmpPeakMz);
}

list<Peak> ConsensusSpectrum::findConsensusPeaks(const list<Peak> &input, const int &peaksToKeep, int &nSpectra){
    // Step 1: merge identical peaks
    list<Peak> ret = mergeIdenticalPeaks(input);
    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    ret = adaptPeakIntensities(ret,nSpectra);
    // Step 3: filter the spectrum
    ret = filterNoise(ret);
    return ret;
}

list<Peak> ConsensusSpectrum::mergeIdenticalPeaks( const list<Peak> &inPeaks) {
    list<Peak> filterdPeaks;
    if(inPeaks.size() == 0) return filterdPeaks;

    filterdPeaks.merge(list<Peak>(inPeaks));

    for(float range = MZ_THRESHOLD_STEP;range <= FINAL_MZ_THRESHOLD;range += MZ_THRESHOLD_STEP){
        list<Peak> newPeakList;
        Peak currentPeak = *filterdPeaks.begin();

        list<Peak>::iterator iter = filterdPeaks.begin();
        iter++;
        for(iter;iter != filterdPeaks.end(); iter++){
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
        filterdPeaks.merge(list<Peak>(newPeakList));
    }
    return filterdPeaks;
}

list<Peak> ConsensusSpectrum::adaptPeakIntensities(const list<Peak> &inp, int nSpectra) {
//    int originalCount;
//    ToDo add PeakUtilities class
//    originalCount = PeakUtilities.getTotalCount(inp);
    list<Peak> ret = inp;
    list<Peak>::iterator iter;
    for(iter = ret.begin();iter != ret.end();iter++){
        Peak peak = *iter;
        float peakProbability = (float)peak.getCount() / (float)nSpectra;
        float newIntensity = (float)(peak.getIntensity() * (0.95 +0.05 * pow(1+peakProbability,5)));

        ret.erase(iter);
        ret.push_back(Peak(peak.getMz(),newIntensity,peak.getCount()));
    }
//    int finalCount;
//    ToDo PeakUtilities;
//    finalCount = PeakUtilities.getTotalCount(ret)
//    if(originalCount != finalCount)
//        throw("Peak merge changed total count");
    return ret;
}

list<Peak> ConsensusSpectrum::filterNoise(const list<Peak> &inp) {
    list<Peak> in = inp;
    in.sort(Peak::cmpPeakMz);
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

void ConsensusSpectrum::addPeaks(const list<Peak> &peaksToAdd) {
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

void ConsensusSpectrum::storeHeldPeaks(const list<Peak> &peaksToAdd) {
    if(nSpectra < SIZE_TO_ADD_EVERY_TIME) throw("cannot add without a fairly large cluster");

    if(lowestConcensusPeak == 0)  getConsensusSpectrum();

    list<Peak> in = peaksToAdd;
    float minimuxKeptPeak = lowestConcensusPeak * FRACTION_OF_LOWEST_PEAK_TOKEEP;

    list<Peak>::iterator iter;
    for(iter = in.begin();iter != peaksToAdd.end();iter++){
        if(iter->getIntensity() > minimuxKeptPeak){
            heldPeaks.insert(Peak(*iter));
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
        list<Peak> emptylist;
        if(consensusSpectrum != nullptr){
            delete consensusSpectrum;
        }
        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),emptylist);
        setDirty(false);
        return;
    }
    list<Peak> newPeaks = findConsensusPeaks(allPeaks,DEFAULT_PEAKS_TO_KEEP,nSpectra);

    consensusPeaks.clear();
    consensusPeaks.merge(list<Peak>(newPeaks));
    float minimumConsensusPeak = 0x1.fffffeP+127f;
    list<Peak>::iterator iter;
    for(iter = consensusPeaks.begin();iter != consensusPeaks.end();iter++){
        float intensity = iter->getIntensity();
        if( intensity < minimumConsensusPeak && intensity > 0) minimumConsensusPeak = intensity;
        lowestConcensusPeak = minimumConsensusPeak;

        if(consensusSpectrum != nullptr){
            delete consensusSpectrum;
        }
        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),consensusPeaks);
        setDirty(false);
    }

}

Spectrum ConsensusSpectrum::getConsensusSpectrum() {
    if(isDirty) update();
    return internalGetConcensusSpectrum();
}
Spectrum ConsensusSpectrum::internalGetConcensusSpectrum() const{
    Spectrum ret(*consensusSpectrum);
    return ret;
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
}


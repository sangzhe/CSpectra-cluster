//
// Created by SangZhe on 16/8/31.
//

#include "ConsensusSpectrum.h"
#include "Peak.h"
#include "Spectrum.h"
#include "Defaults.h"


int ConsensusSpectrum::DEFAULT_PEAKS_TO_KEEP = 5;

int ConsensusSpectrum::SIZE_TO_ADD_EVERY_TIME = 100;

float ConsensusSpectrum::NOISE_FILTER_INCREMENT = 100;

// ToDo Functions
//static IFunction<List<IPeak>, List<IPeak>> noiseFilter = new BinnedHighestNPeakFunction(DEFAULT_PEAKS_TO_KEEP, (int) NOISE_FILTER_INCREMENT, 0);

float ConsensusSpectrum::FRACTION_OF_LOWEST_PEAK_TOKEEP = 0.40F;

const string methodName = "Crowded Consensus Spectrum Builder";
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

ConsensusSpectrum::ConsensusSpectrum(string &id, int &nSpectra, float &sumPrecusorMz, float &sumPrecursorIntens,
                                     int &sumCharge, list<IPeak*> &peaks) {
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecusorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;
    consensusPeaks.merge(peaks);

    setDirty(true);
}

list<IPeak*> ConsensusSpectrum::getInternalPeaks() {
    return consensusPeaks;
}

void ConsensusSpectrum::addSpectra(list<ISpectrum*> &spectra) {
    if(spectra.size() < 1) return;

    list<ISpectrum*>::iterator iter;
    for(iter = spectra.begin();iter != spectra.end();iter++){
        addSpectra(*(*iter));
    }
    setDirty(true);

}

void ConsensusSpectrum::addSpectra(ISpectrum &merged) {
    list<IPeak*> spectrumPeaks = merged.getPeaks();
    addPeaks(spectrumPeaks);
    sumCharge += merged.getPrecursorCharge();
    sumPrecursorMz += merged.getPrecursorMz();
    sumPrecursorIntens += 0;

    nSpectra++;
}

void ConsensusSpectrum::removeSpectra(list<ISpectrum *> &spectra) {
    if(spectra.size() < 1) return;
    list<ISpectrum*>::iterator iter;
    for(iter = spectra.begin();iter != spectra.end();iter++){
        removeSpectra(*(*iter));
}
    setDirty(true);

}

void ConsensusSpectrum::removeSpectra(ISpectrum &removed) {
    list<IPeak *> spectrumPeaks = removed.getPeaks();
    removePeaks(spectrumPeaks);

    sumCharge -= removed.getPrecursorCharge();
    sumPrecursorMz -= removed.getPrecursorMz();
    sumPrecursorIntens -= 0;

    nSpectra--;
}

void ConsensusSpectrum::removePeaks(list<IPeak *> &peaksToRemove) {

}

void ConsensusSpectrum::addHeldPeaks() {
    list<IPeak*> added;
    set<IPeak*>::iterator iter;
    for(iter = heldPeaks.begin();iter != heldPeaks.end();iter++){
        added.push_back(*iter);
    }
    added.sort(Peak::cmpPeak);
    internalAddPeaks(added);
    heldPeaks.clear();
}

void ConsensusSpectrum::internalAddPeaks(list<IPeak *> &peaksToAdd) {
    list<IPeak*> *newPeaks = new list<IPeak*>;

    list<IPeak*>::iterator iter;
    for(iter = peaksToAdd.begin() ;iter != peaksToAdd.end();iter++){
        IPeak* peakToAdd = *iter;
        double mzToAdd = peakToAdd->getMz();
        if(USE_ROUNDING){
//       ToDo add MZIntensity class
//            mzToAdd = MZIntensityUtilities.round(mzToAdd, MZ_PRECISION);
        }
        bool wasAdded = false;

        list<IPeak*>::iterator iter1;
        for(iter1 = allPeaks.begin();iter1 != allPeaks.end();iter1++){
            IPeak* currentExistingPeak = *iter1;
            if(mzToAdd < currentExistingPeak->getMz()){
                newPeaks->push_back(new Peak((float)mzToAdd,peakToAdd->getIntensity(),peakToAdd->getCount()));
                wasAdded = true;
                break;
            }
            if(mzToAdd == currentExistingPeak->getMz()){
                allPeaks.erase(iter);
                allPeaks.push_back(new Peak(currentExistingPeak->getMz(),peakToAdd->getIntensity()+currentExistingPeak->getIntensity(),currentExistingPeak->getCount()+peakToAdd->getCount()));
                wasAdded = true;
                break;
            }
        }
        if(!wasAdded)
            newPeaks->push_back(new Peak((float)mzToAdd,peakToAdd->getIntensity(),peakToAdd->getCount()));

        allPeaks.merge(*newPeaks);
        allPeaks.sort(Peak::cmpPeakMz);
    }
}

list<IPeak*> ConsensusSpectrum::findConsensusPeaks(list<IPeak *> &input, const int &peaksToKeep, int &nSpectra) {
    // Step 1: merge identical peaks
    list<IPeak*> ret = mergeIdenticalPeaks(input);
    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    ret = adaptPeakIntensities(ret,nSpectra);
    // Step 3: filter the spectrum
    ret = filterNoise(ret);
    return ret;
}

list<IPeak*> ConsensusSpectrum::mergeIdenticalPeaks(list<IPeak *> &inPeaks) {
    list<IPeak*> filterdPeaks;
    if(inPeaks.size() == 0) return filterdPeaks;

    filterdPeaks.merge(inPeaks);

    for(float range = MZ_THRESHOLD_STEP;range <= FINAL_MZ_THRESHOLD;range += MZ_THRESHOLD_STEP){
        list<IPeak*> *newPeakList = new list<IPeak*>;
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

                IPeak *newPeak = new Peak((float)weightedMz,(float)intensity,count);

                delete currentPeak;
                currentPeak = newPeak;
            }else{
                newPeakList->push_back(currentPeak);
                delete currentPeak;
                currentPeak = nextPeak;

            }
        }
        newPeakList->push_back(currentPeak);
        filterdPeaks.clear();
        filterdPeaks.merge(*newPeakList);
        delete newPeakList;newPeakList = nullptr;
    }
    return filterdPeaks;
}

list<IPeak*> ConsensusSpectrum::adaptPeakIntensities(list<IPeak *> &inp, int nSpectra) {
    int originalCount;
//    ToDo add PeakUtilities class
//    originalCount = PeakUtilities.getTotalCount(inp);
    list<IPeak*> ret = inp;
    list<IPeak*>::iterator iter;
    for(iter = ret.begin();iter != ret.end();iter++){
        IPeak* peak = *iter;
        float peakProbability = (float)peak->getCount() / (float)nSpectra;
        float newIntensity = (float)(peak->getIntensity() * (0.95 +0.05 * pow(1+peakProbability,5)));

        ret.erase(iter);
        ret.push_back(new Peak(peak->getMz(),newIntensity,peak->getCount()));
    }
    int finalCount;
//    ToDo PeakUtilities;
//    finalCount = PeakUtilities.getTotalCount(ret)
    if(originalCount != finalCount)
        throw("Peak merge changed total count");
    return ret;
}

list<IPeak*> ConsensusSpectrum::filterNoise(list<IPeak *> &inp) {
    inp.sort(Peak::cmpPeakMz);
    list<IPeak*> filteredSpectrum;
//    ToDo IFunctions class: nosiFilter
//    filteredSpectrum = noiseFilter(inp);

    return filteredSpectrum;
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

void ConsensusSpectrum::addPeaks(list<IPeak *> &peaksToAdd) {
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

void ConsensusSpectrum::storeHeldPeaks(list<IPeak *> &peaksToAdd) {
    if(nSpectra < SIZE_TO_ADD_EVERY_TIME) throw("cannot add without a fairly large cluster");

    if(lowestConcensusPeak == 0)  getConsensusSpectrum();

    float minimuxKeptPeak = lowestConcensusPeak * FRACTION_OF_LOWEST_PEAK_TOKEEP;

    list<IPeak*>::iterator iter;
    for(iter = peaksToAdd.begin();iter != peaksToAdd.end();iter++){
        if((*iter)->getIntensity() > minimuxKeptPeak){
            heldPeaks.insert(*iter);
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
        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),emptylist);
        setDirty(false);
        return;
    }
    list<IPeak*> newPeaks = findConsensusPeaks(allPeaks,DEFAULT_PEAKS_TO_KEEP,nSpectra);

    consensusPeaks.clear();
    consensusPeaks.merge(newPeaks);
    float minimumConsensusPeak = 0x1.fffffeP+127f;
    list<IPeak*>::iterator iter;
    for(iter = consensusPeaks.begin();iter != consensusPeaks.end();iter++){
        float intensity = (*iter)->getIntensity();
        if( intensity < minimumConsensusPeak && intensity > 0) minimumConsensusPeak = intensity;
        lowestConcensusPeak = minimumConsensusPeak;

        consensusSpectrum = new Spectrum(id,averageCharge,averagePrecursorMz,Defaults::getDefaultQualityScorer(),consensusPeaks);
        setDirty(false);
    }

}

ISpectrum* ConsensusSpectrum::getConsensusSpectrum() {
    if(isDirty) update();
    return internalGetConcensusSpectrum();
}
ISpectrum* ConsensusSpectrum::internalGetConcensusSpectrum() {
    return consensusSpectrum;
}

int ConsensusSpectrum::getSumCharge() {
    return sumCharge;
}

double ConsensusSpectrum::getSumPrecursorMz() {
    return sumPrecursorMz;
}

double ConsensusSpectrum::getSumPrecursorIntensity() {
    return sumPrecursorIntens;
}

int ConsensusSpectrum::getSpectraCount() {
    return nSpectra;
}

bool ConsensusSpectrum::isRemovedSupported() {
    return true;
}

void ConsensusSpectrum::setDirty(bool isDirty) {
    this->isDirty = isDirty;
}
ConsensusSpectrum::~ConsensusSpectrum() {
    delete FACTORY,consensusSpectrum;
}


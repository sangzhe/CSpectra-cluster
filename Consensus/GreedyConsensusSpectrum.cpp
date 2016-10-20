//
// Created by SangZhe on 16/9/9.
//

#include "GreedyConsensusSpectrum.h"


int GreedyConsensusSpectrum::DEFAULT_PEAKS_TO_KEEP = 5;

float GreedyConsensusSpectrum::NOISE_FILTER_INCREMENT = 100;

PointerPool* GreedyConsensusSpectrum::pointer_pool = PoolFactory::getInstance();


GreedyConsensusSpectrumFactory* GreedyConsensusSpectrum::FACTORY = new GreedyConsensusSpectrumFactory();

ConsensusSpectrumBuilderFactory* GreedyConsensusSpectrum::buildFactory() {
    return new GreedyConsensusSpectrumFactory();
}

GreedyConsensusSpectrum::GreedyConsensusSpectrum(float fragmentTolerance) {
    new (this)GreedyConsensusSpectrum(fragmentTolerance,"");
};

GreedyConsensusSpectrum::GreedyConsensusSpectrum(float fragmentTolerance, string id) {
    this->fragmentTolerance = fragmentTolerance;
    this->id = id;
};


GreedyConsensusSpectrum::GreedyConsensusSpectrum(float fragmentTolerance, string id, int nSpectra,
                                                 double sumPrecursorMz, double sumPrecursorIntens, int sumCharge,
                                                 const vector<Peak> peaks) {
    this->fragmentTolerance = fragmentTolerance;
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecursorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;

    // update properties charge, precursor m/z and precursor intensity
    updateProperties();

    for(const Peak& peak:peaks){
        consensusPeaks.push_back(peak);
    }

    setIsDirty(true);

};

void GreedyConsensusSpectrum::addSpectra( vector<ISpectrum*> &spectra) {

    if(spectra.size() < 1) return;
    vector<ISpectrum*>::iterator iter;
    for(ISpectrum* spectrum: spectra){
        addSpectra(spectrum);
    }
    updateProperties();

    setIsDirty(true);

    for (SpectrumHolderListener *listener : listeners)
        listener->onSpectraAdd(this,spectra);

}

void GreedyConsensusSpectrum::addSpectra( ISpectrum *merged) {
    vector<Peak> spectrumPeaks = merged->getPeaks();
    addPeaksToConsensus(spectrumPeaks); // peaks are added but not additional transformation is done

    // merge identical peaks
    vector<Peak> mergedPeaks = mergeIdenticalPeaks(consensusPeaks);
    consensusPeaks.clear();
    consensusPeaks = mergedPeaks;

    sumCharge += merged->getPrecursorCharge();
    sumPrecursorMz += merged->getPrecursorMz();
    sumPrecursorIntens += 0;
    nSpectra++;
}

void GreedyConsensusSpectrum::addConsensusSpectrum( IConsensusSpectrumBuilder &consensusSpectrumToAdd) {
    if(consensusSpectrumToAdd.getSpectraCount() < 1) return;

    // add the peaks like in a "normal" spectrum - the peak count's are preserved

    vector<Peak> peaks = consensusSpectrumToAdd.getConsensusSpectrum()->getPeaks();
    addPeaksToConsensus(peaks);

    // merge identical peaks
    vector<Peak> mergedPeaks = mergeIdenticalPeaks(consensusPeaks);
    consensusPeaks.clear();
    for(Peak& peak:mergedPeaks){
        consensusPeaks.push_back(peak);
    }
    // update the general properties
    sumCharge += consensusSpectrumToAdd.getSumCharge();
    sumPrecursorMz += consensusSpectrumToAdd.getSumPrecursorMz();
    sumPrecursorIntens += consensusSpectrumToAdd.getSumPrecursorIntensity();
    nSpectra += consensusSpectrumToAdd.getSpectraCount();

    // update properties charge, precursor m/z and precursor intensity
    updateProperties();

    setIsDirty(true);

    for (SpectrumHolderListener *listener : listeners){
        ISpectrum* consensus = consensusSpectrumToAdd.getConsensusSpectrum();
        vector<ISpectrum*> tmp;
        tmp.push_back(consensus);
        listener->onSpectraAdd(this, tmp);
    }

}

void GreedyConsensusSpectrum::updateConsensusSpectrum() {
    if (isDirty()) {

        // update the actual consensus spectrum
        vector<Peak> processedConsensusPeaks = findConsensusPeaks(consensusPeaks, nSpectra);
        if (consensusSpectrum != nullptr) delete consensusSpectrum;
        consensusSpectrum = nullptr;
        consensusSpectrum = new Spectrum(id, averageCharge, averagePrecursorMz, Defaults::getDefaultQualityScorer(), processedConsensusPeaks);
        setIsDirty(false);
    }
}

void GreedyConsensusSpectrum::removeSpectra( vector<ISpectrum*> &spectra) {
    throw("GreedyConsensusSpectrum does not support removing of spectra.");
}

void GreedyConsensusSpectrum::removeSpectra( ISpectrum *removed) {
    throw("GreedyConsensusSpectrum does not support removing of spectra.");

}

bool GreedyConsensusSpectrum::isRemovedSupported() {
    return false;
}

void GreedyConsensusSpectrum::addPeaksToConsensus(const vector<Peak> &peaksToAdd) {
    int posAllPeaks = 0;
    vector<Peak> newPeaks;
    for(const Peak &p: peaksToAdd){
        Peak peakToAdd = p;
        float mzToAdd = peakToAdd.getMz();
        bool wasAdded = false;
        for(int j = posAllPeaks;j<consensusPeaks.size();j++){
            Peak currentExistingPeak = consensusPeaks[j];

            if (mzToAdd < currentExistingPeak.getMz()) {
                Peak newPeak (mzToAdd, peakToAdd.getIntensity(), peakToAdd.getCount());
                newPeaks.push_back(newPeak);
                posAllPeaks = j;
                wasAdded = true;
                break;
            }

            if (mzToAdd == currentExistingPeak.getMz()) {
                Peak newPeak (currentExistingPeak.getMz(), peakToAdd.getIntensity() + currentExistingPeak.getIntensity(),
                        currentExistingPeak.getCount() + peakToAdd.getCount());
                consensusPeaks[j] = newPeak;
                posAllPeaks = j;
                wasAdded = true;
                break;
            }
        }
        if(!wasAdded){
            Peak newPeak (mzToAdd, peakToAdd.getIntensity(), peakToAdd.getCount());
            newPeaks.push_back(newPeak);

        }
    }
    for(Peak& peak:newPeaks){
        consensusPeaks.push_back(peak);
    }
    sort(consensusPeaks.begin(),consensusPeaks.end(),Peak::cmpPeakMz);
}

void GreedyConsensusSpectrum::updateProperties() {
    if (nSpectra > 0) {
        averagePrecursorMz = (float) sumPrecursorMz / nSpectra;
        averageCharge = sumCharge / nSpectra;
        averagePrecursorIntens = (float) sumPrecursorIntens / nSpectra;
    } else {
        averagePrecursorMz = 0;
        averageCharge = 0;
        averagePrecursorIntens = 0;
    }
}

vector<Peak> GreedyConsensusSpectrum::findConsensusPeaks(const vector<Peak> &input, int nSpectra) {
    vector<Peak> peaks = input;
    if (peaks.size() < 1)
        return peaks;

    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    vector<Peak> ret = adaptPeakIntensities(peaks, nSpectra);

    // Step 3: filter the spectrum
    ret = filterNoise(ret);

    return ret;
}

vector<Peak> GreedyConsensusSpectrum::filterNoise(const vector<Peak> &inp) {
    vector<Peak> in = inp;
    sort(in.begin(),in.end(),Peak::cmpPeakMz);
    BinnedHighestNPeakFunction noiseFilter = BinnedHighestNPeakFunction(DEFAULT_PEAKS_TO_KEEP, (int) NOISE_FILTER_INCREMENT, 0);
    in = noiseFilter.apply(in);
    return in;

}

vector<Peak> GreedyConsensusSpectrum::adaptPeakIntensities(const vector<Peak> &inp, int nSpectra) {
    vector<Peak> in = inp;
    for (int i = 0; i < in.size(); i++) {
        Peak peak = in[i];
        float peakProbability = (float) peak.getCount() / (float) nSpectra;
        float newIntensity = (float) (peak.getIntensity() * (0.95 + 0.05 * pow(1 + peakProbability, 5)));

        Peak newPeak (peak.getMz(), newIntensity, peak.getCount());
        in[i] = newPeak;
    }
    return in;
}

vector<Peak> GreedyConsensusSpectrum::mergeIdenticalPeaks(const vector<Peak> &inPeaks) const{
    vector<Peak> filteredPeaks = inPeaks;
    if(inPeaks.size() == 0) return vector<Peak>();

    float mzThresholdStep = fragmentTolerance / 4; // use 4 rounds to reach the final mz threshold
    for(float range = mzThresholdStep;range <= fragmentTolerance;range += mzThresholdStep) {
        vector<Peak> newPeakList;

        Peak currentPeak = filteredPeaks[0];

        for (int i = 1; i < filteredPeaks.size(); i++) {
            Peak nextPeak = filteredPeaks[i];

            const float nextPeakMz = nextPeak.getMz();
            const float currentPeakMz = currentPeak.getMz();
            const float testLimit = currentPeakMz + range;

            if (nextPeakMz <= testLimit) {
                const double nextPeakIntensity = nextPeak.getIntensity();
                const double currentPeakIntensity = currentPeak.getIntensity();
                const double totalIntensity = nextPeakIntensity + currentPeakIntensity;
                const double nextPeakFraction = nextPeakIntensity / totalIntensity;
                const double currentPeakFraction = currentPeakIntensity / totalIntensity;

                double weightedMz = (nextPeakFraction * nextPeakMz) + (currentPeakFraction * currentPeakMz);

                const double intensity = currentPeakIntensity + nextPeakIntensity;

                const int count = currentPeak.getCount() + nextPeak.getCount();

                Peak newPeak((float) weightedMz, (float) intensity, count);
                currentPeak = newPeak;

            } else {
                newPeakList.push_back(currentPeak);
                currentPeak = nextPeak;
            }
        }
        newPeakList.push_back(currentPeak);
        filteredPeaks.clear();
        for (Peak &peak:newPeakList) {
            filteredPeaks.push_back(peak);
        }
    }
    return filteredPeaks;
}

ISpectrum* GreedyConsensusSpectrum::getConsensusSpectrum() {
    updateConsensusSpectrum();
    return consensusSpectrum;
}

void GreedyConsensusSpectrum::clear() {
    sumCharge = 0;
    sumPrecursorMz = 0;
    sumPrecursorIntens = 0;
    nSpectra = 0;

    consensusPeaks.clear();
    setIsDirty(true);
}

void GreedyConsensusSpectrum::onSpectraAdd(ISpectrumHolder *holder, vector<ISpectrum *> &added) {
//        pointer_pool->add(added);
        addSpectra(added);

}

void GreedyConsensusSpectrum::onSpectraRemove(ISpectrumHolder *holder, vector<ISpectrum *> &removed) {
    removeSpectra(removed);
//    pointer_pool->remove(removed);
}

void GreedyConsensusSpectrum::addSpectrumHolderListener(SpectrumHolderListener *added) {
    listeners.push_back(added);
    pointer_pool->add(added);

}

void GreedyConsensusSpectrum::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(listeners.begin(),listeners.end(),removed));
    if(iter != listeners.end()){
        IPointer* R = *iter;
        listeners.erase(iter);
        pointer_pool->remove(R);
    }
}

int GreedyConsensusSpectrum::getSpectraCount()const {
    return nSpectra;
}

int GreedyConsensusSpectrum::getSumCharge()const {
    return sumCharge;
}

double GreedyConsensusSpectrum::getSumPrecursorIntensity() const{
    return sumPrecursorIntens;
}

double GreedyConsensusSpectrum::getSumPrecursorMz() const{
    return sumPrecursorMz;
}
bool GreedyConsensusSpectrum::isDirty() {
    return Dirty;
}

void GreedyConsensusSpectrum::setIsDirty(bool isDirty) {
    this->Dirty = isDirty;
}

string GreedyConsensusSpectrum::getMethodName() {
    return methodName;
}

GreedyConsensusSpectrum::~GreedyConsensusSpectrum() {
    delete FACTORY,consensusSpectrum;
    FACTORY = nullptr;
    consensusSpectrum = nullptr;
    for(SpectrumHolderListener* listener:listeners){
        pointer_pool->remove(listener);
    }
}
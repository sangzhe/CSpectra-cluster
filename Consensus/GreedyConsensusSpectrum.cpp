//
// Created by SangZhe on 16/9/9.
//

#include "GreedyConsensusSpectrum.h"

int GreedyConsensusSpectrum::DEFAULT_PEAKS_TO_KEEP = 5;

float GreedyConsensusSpectrum::NOISE_FILTER_INCREMENT = 100;

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
                                                 const vector<IPeak*> peaks) {
    this->fragmentTolerance = fragmentTolerance;
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecursorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;

    // update properties charge, precursor m/z and precursor intensity
    updateProperties();

    this->consensusPeaks.insert(consensusPeaks.end(),peaks.begin(),peaks.end());
    PointerPool::add(peaks);

    setIsDirty(true);

};

void GreedyConsensusSpectrum::addSpectra(const vector<ISpectrum*> &spectra) {
    if(spectra.size() < 1) return;
    vector<ISpectrum*> in = spectra;
    vector<ISpectrum*>::iterator iter;
    for(iter = in.begin();iter != in.end();iter++){
        addSpectra(*iter);
    }
    updateProperties();

    setIsDirty(true);

    for (SpectrumHolderListener *listener : listeners)
        listener->onSpectraAdd(this,in);

}

void GreedyConsensusSpectrum::addSpectra(const ISpectrum *merged) {
    vector<IPeak*> spectrumPeaks = merged->getPeaks();
    addPeaksToConsensus(spectrumPeaks); // peaks are added but not additional transformation is done

    // merge identical peaks
    vector<IPeak*> mergedPeaks = mergeIdenticalPeaks(consensusPeaks);
    consensusPeaks.clear();
    consensusPeaks.insert(consensusPeaks.end(),mergedPeaks.begin(),mergedPeaks.end());

    sumCharge += merged->getPrecursorCharge();
    sumPrecursorMz += merged->getPrecursorMz();
    sumPrecursorIntens += 0;
    nSpectra++;
}

void GreedyConsensusSpectrum::addConsensusSpectrum( IConsensusSpectrumBuilder &consensusSpectrumToAdd) {
    if(consensusSpectrumToAdd.getSpectraCount() < 1) return;

    // add the peaks like in a "normal" spectrum - the peak count's are preserved

    vector<IPeak*> peaks = consensusSpectrumToAdd.getConsensusSpectrum()->getPeaks();
    addPeaksToConsensus(peaks);

    // merge identical peaks
    vector<IPeak*> mergedPeaks = mergeIdenticalPeaks(consensusPeaks);
    consensusPeaks.clear();
    consensusPeaks.insert(consensusPeaks.end(),mergedPeaks.begin(),mergedPeaks.end());

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
        vector<IPeak*> processedConsensusPeaks = findConsensusPeaks(consensusPeaks, nSpectra);
        delete consensusSpectrum;
        consensusSpectrum = nullptr;
        consensusSpectrum = new Spectrum(id, averageCharge, averagePrecursorMz, Defaults::getDefaultQualityScorer(), processedConsensusPeaks);
        setIsDirty(false);
    }
}

void GreedyConsensusSpectrum::removeSpectra(const vector<ISpectrum*> &spectra) {
    throw("GreedyConsensusSpectrum does not support removing of spectra.");
}

void GreedyConsensusSpectrum::removeSpectra(const ISpectrum *removed) {
    throw("GreedyConsensusSpectrum does not support removing of spectra.");

}

bool GreedyConsensusSpectrum::isRemovedSupported() {
    return false;
}

void GreedyConsensusSpectrum::addPeaksToConsensus(const vector<IPeak*> &peaksToAdd) {
    int posAllPeaks = 0;
    vector<IPeak*> newPeaks;
    for(IPeak* p: peaksToAdd){
        IPeak* peakToAdd = p;
        float mzToAdd = peakToAdd->getMz();
        bool wasAdded = false;
        for(int j = posAllPeaks;j<consensusPeaks.size();j++){
            IPeak *currentExistingPeak = consensusPeaks[j];

            if (mzToAdd < currentExistingPeak->getMz()) {
                IPeak *newPeak = new Peak(mzToAdd, peakToAdd->getIntensity(), peakToAdd->getCount());
                newPeaks.push_back(newPeak);
                PointerPool::add(newPeak);
                posAllPeaks = j;
                wasAdded = true;
                break;
            }

            if (mzToAdd == currentExistingPeak->getMz()) {
                IPeak *newPeak = new Peak(currentExistingPeak->getMz(), peakToAdd->getIntensity() + currentExistingPeak->getIntensity(),
                        currentExistingPeak->getCount() + peakToAdd->getCount());
                PointerPool::add(newPeak);
                PointerPool::remove(currentExistingPeak);consensusPeaks[j] = newPeak;
                posAllPeaks = j;
                wasAdded = true;
                break;
            }
        }
        if(!wasAdded){
            IPeak *newPeak = new Peak(mzToAdd, peakToAdd->getIntensity(), peakToAdd->getCount());
            newPeaks.push_back(newPeak);
            PointerPool::add(newPeak);

        }
    }
    consensusPeaks.insert(consensusPeaks.end(),newPeaks.begin(),newPeaks.end());
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

vector<IPeak*> GreedyConsensusSpectrum::findConsensusPeaks(const vector<IPeak*> &input, int nSpectra) {
    if (input.size() < 1)
        return input;

    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    vector<IPeak*> ret = adaptPeakIntensities(input, nSpectra);

    // Step 3: filter the spectrum
    ret = filterNoise(ret);

    return ret;
}

vector<IPeak*> GreedyConsensusSpectrum::filterNoise(const vector<IPeak*> &inp) {
    vector<IPeak*> in = inp;
    sort(in.begin(),in.end(),Peak::cmpPeakMz);
    BinnedHighestNPeakFunction noiseFilter;
    in = noiseFilter.apply(in);
    return in;

}

vector<IPeak*> GreedyConsensusSpectrum::adaptPeakIntensities(const vector<IPeak*> &inp, int nSpectra) {
    vector<IPeak*> peaks(inp.begin(),inp.end());
    for (int i = 0; i < peaks.size(); i++) {
        IPeak *peak = peaks[i];
        float peakProbability = (float) peak->getCount() / (float) nSpectra;
        float newIntensity = (float) (peak->getIntensity() * (0.95 + 0.05 * pow(1 + peakProbability, 5)));

        IPeak* newPeak = new Peak(peak->getMz(), newIntensity, peak->getCount());
        peaks[i] = newPeak;
        PointerPool::add(newPeak);
        PointerPool::remove(peak);
    }
    vector<IPeak*> ret(peaks.begin(),peaks.end());
    return ret;
}

vector<IPeak*> GreedyConsensusSpectrum::mergeIdenticalPeaks(const vector<IPeak*> &inPeaks) const{
    vector<IPeak*> filterdPeaks;
    if(inPeaks.size() == 0) return filterdPeaks;

    filterdPeaks = inPeaks;
    float mzThresholdStep = fragmentTolerance / 4; // use 4 rounds to reach the final mz threshold
    for(float range = mzThresholdStep;range <= mzThresholdStep;range += mzThresholdStep){
        vector<IPeak*> newPeakList;

        vector<IPeak*>::iterator iter = filterdPeaks.begin();
        IPeak* currentPeak = *iter;

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

                IPeak* newPeak = new Peak((float)weightedMz,(float)intensity,count);
                currentPeak = newPeak;

            }else{
                PointerPool::add(currentPeak);
                newPeakList.push_back(currentPeak);
                currentPeak = nextPeak;


            }
        }
        PointerPool::add(currentPeak);
        newPeakList.push_back(currentPeak);
        PointerPool::remove(filterdPeaks);
        filterdPeaks.clear();
        filterdPeaks.insert(filterdPeaks.end(),newPeakList.begin(),newPeakList.end());
    }
    return filterdPeaks;
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
    addSpectra(added);
    PointerPool::add(added);
}

void GreedyConsensusSpectrum::onSpectraRemove(ISpectrumHolder *holder, vector<ISpectrum *> &removed) {
    removeSpectra(removed);
    PointerPool::remove(removed);
}

void GreedyConsensusSpectrum::addSpectrumHolderListener(SpectrumHolderListener *added) {
    listeners.push_back(added);
    PointerPool::add(added);

}

void GreedyConsensusSpectrum::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(listeners.begin(),listeners.end(),removed));
    if(iter != listeners.end()){
        listeners.erase(iter);
        PointerPool::remove(*iter);
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
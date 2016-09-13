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
                                                 const list<Peak> peaks) {
    this->fragmentTolerance = fragmentTolerance;
    this->id = id;
    this->nSpectra = nSpectra;
    this->sumPrecursorMz = sumPrecursorMz;
    this->sumPrecursorIntens = sumPrecursorIntens;
    this->sumCharge = sumCharge;

    // update properties charge, precursor m/z and precursor intensity
    updateProperties();

    this->consensusPeaks.merge(list<Peak>(peaks));

    setIsDirty(true);

};

void GreedyConsensusSpectrum::addSpectra(const list<Spectrum> &spectra) {
    if(spectra.size() < 1) return;
    list<Spectrum> in = spectra;
    list<Spectrum>::iterator iter;
    for(iter = in.begin();iter != in.end();iter++){
        addSpectra(*iter);
    }
    updateProperties();

    setIsDirty(true);

//     ToDo SpectrumHolderListener
}

void GreedyConsensusSpectrum::addSpectra(const Spectrum &merged) {
    list<Peak> spectrumPeaks = merged.getPeaks();
    addPeaksToConsensus(spectrumPeaks); // peaks are added but not additional transformation is done

    // merge identical peaks
    list<Peak> mergedPeaks = mergeIdenticalPeaks(consensusPeaks);
    consensusPeaks.clear();
    consensusPeaks.merge(list<Peak>(mergedPeaks));

    sumCharge += merged.getPrecursorCharge();
    sumPrecursorMz += merged.getPrecursorMz();
    sumPrecursorIntens += 0;
    nSpectra++;
}

void GreedyConsensusSpectrum::addConsensusSpectrum( IConsensusSpectrumBuilder &consensusSpectrumToAdd) {
    if(consensusSpectrumToAdd.getSpectraCount() < 1) return;

    // add the peaks like in a "normal" spectrum - the peak count's are preserved

    list<Peak> peaks = consensusSpectrumToAdd.getConsensusSpectrum().getPeaks();
    addPeaksToConsensus(peaks);

    // merge identical peaks
    list<Peak> mergedPeaks = mergeIdenticalPeaks(consensusPeaks);
    consensusPeaks.clear();
    consensusPeaks.merge(mergedPeaks);

    // update the general properties
    sumCharge += consensusSpectrumToAdd.getSumCharge();
    sumPrecursorMz += consensusSpectrumToAdd.getSumPrecursorMz();
    sumPrecursorIntens += consensusSpectrumToAdd.getSumPrecursorIntensity();
    nSpectra += consensusSpectrumToAdd.getSpectraCount();

    // update properties charge, precursor m/z and precursor intensity
    updateProperties();

    setIsDirty(true);

    //     ToDo SpectrumHolderListener

}

void GreedyConsensusSpectrum::updateConsensusSpectrum() {
    if (isDirty()) {

        // update the actual consensus spectrum
        list<Peak> processedConsensusPeaks = findConsensusPeaks(consensusPeaks, nSpectra);
        delete consensusSpectrum;
        consensusSpectrum = new Spectrum(id, averageCharge, averagePrecursorMz, Defaults::getDefaultQualityScorer(), processedConsensusPeaks);
        setIsDirty(false);
    }
}

void GreedyConsensusSpectrum::removeSpectra(const list<Spectrum> &spectra) {
    throw("GreedyConsensusSpectrum does not support removing of spectra.");
}

void GreedyConsensusSpectrum::removeSpectra(const Spectrum &removed) {
    throw("GreedyConsensusSpectrum does not support removing of spectra.");

}

bool GreedyConsensusSpectrum::isRemovedSupported() {
    return false;
}

void GreedyConsensusSpectrum::addPeaksToConsensus(const list<Peak> &peaksToAdd) {
    list<Peak> in = peaksToAdd;
    list<Peak> newPeaks;

    list<Peak>::iterator iterator1;

    list<Peak> PeaksToUpdate ;

    for(iterator1 = in.begin();iterator1 != in.end();iterator1++){
        Peak peakToAdd = *iterator1;
        float mzToAdd = peakToAdd.getMz();
        bool wasAdded = false;
        list<Peak>::iterator iterator2;
        for(iterator2 = consensusPeaks.begin();iterator2 != consensusPeaks.end();){
            Peak currentExistingPeak = *iterator2;

            if (mzToAdd < currentExistingPeak.getMz()) {
                newPeaks.push_back( Peak(mzToAdd, peakToAdd.getIntensity(), peakToAdd.getCount()));
                iterator2++;
                wasAdded = true;
                break;
            }
            if (mzToAdd == currentExistingPeak.getMz()) {
                PeaksToUpdate.push_back(*iterator2);
                consensusPeaks.push_back(Peak(currentExistingPeak.getMz(), peakToAdd.getIntensity() + currentExistingPeak.getIntensity(),
                        currentExistingPeak.getCount() + peakToAdd.getCount()));
                iterator2++;
                wasAdded = true;
                break;

            }
        }
        if (!wasAdded)
            newPeaks.push_back(Peak(mzToAdd, peakToAdd.getIntensity(), peakToAdd.getCount()));
    }
    for(iterator1 = PeaksToUpdate.begin();iterator1 != PeaksToUpdate.end();iterator1++){
        consensusPeaks.erase(iterator1);
    }
    consensusPeaks.merge(newPeaks);
    consensusPeaks.sort(Peak::cmpPeakMz);
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

list<Peak> GreedyConsensusSpectrum::findConsensusPeaks(const list<Peak> &input, int nSpectra) {
    if (input.size() < 1)
        return input;

    // Step 2: adapt the peak intensities based on the probability that the peak has been observed
    list<Peak> ret = adaptPeakIntensities(input, nSpectra);

    // Step 3: filter the spectrum
    ret = filterNoise(ret);

    return ret;
}

list<Peak> GreedyConsensusSpectrum::filterNoise(const list<Peak> &inp) {
    list<Peak> in = inp;
    in.sort(Peak::cmpPeakMz);
    BinnedHighestNPeakFunction noiseFilter;
    in = noiseFilter.apply(in);
    return in;

}

list<Peak> GreedyConsensusSpectrum::adaptPeakIntensities(const list<Peak> &inp, int nSpectra) {
    list<Peak> ret = inp;
    list<Peak>::iterator iter;
    list<Peak> peaksToUpdate;
    for(iter = ret.begin();iter != ret.end();iter++){
        Peak peak = *iter;
        float peakProbability = (float)peak.getCount() / (float)nSpectra;
        float newIntensity = (float)(peak.getIntensity() * (0.95 +0.05 * pow(1+peakProbability,5)));

        peaksToUpdate.push_back(*iter);
        ret.push_back( Peak(peak.getMz(),newIntensity,peak.getCount()));
    }

    for(iter = peaksToUpdate.begin();iter != peaksToUpdate.end();iter++){
        ret.erase(iter);
    }

    return ret;
}

list<Peak> GreedyConsensusSpectrum::mergeIdenticalPeaks(const list<Peak> &inPeaks) const{
    list<Peak> filterdPeaks;
    if(inPeaks.size() == 0) return filterdPeaks;

    filterdPeaks = inPeaks;
    float mzThresholdStep = fragmentTolerance / 4; // use 4 rounds to reach the final mz threshold
    for(float range = mzThresholdStep;range <= mzThresholdStep;range += mzThresholdStep){
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

                Peak newPeak = Peak((float)weightedMz,(float)intensity,count);

                currentPeak = newPeak;
            }else{
                newPeakList.push_back(currentPeak);
                currentPeak = nextPeak;

            }
        }
        newPeakList.push_back(currentPeak);
        filterdPeaks.clear();
        filterdPeaks.merge(newPeakList);
    }
    return filterdPeaks;
}

Spectrum GreedyConsensusSpectrum::getConsensusSpectrum() {
    updateConsensusSpectrum();
    return *consensusSpectrum;
}

void GreedyConsensusSpectrum::clear() {
    sumCharge = 0;
    sumPrecursorMz = 0;
    sumPrecursorIntens = 0;
    nSpectra = 0;

    consensusPeaks.clear();
    setIsDirty(true);
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
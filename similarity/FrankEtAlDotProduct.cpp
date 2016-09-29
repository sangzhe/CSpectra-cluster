//
// Created by SangZhe on 2016/9/27.
//

#include "FrankEtAlDotProduct.h"

int FrankEtAlDotProduct::K2011_BIN_SIZE = 50;

int FrankEtAlDotProduct::DEFAULT_NUMBER_OF_PEAKS_TO_COMPARE = 15;

bool FrankEtAlDotProduct::DEFAULT_PEAK_FILTERING = false;

FrankEtAlDotProduct::FrankEtAlDotProduct() {
    new(this)FrankEtAlDotProduct(Defaults::getFragmentIonTolerance(), DEFAULT_NUMBER_OF_PEAKS_TO_COMPARE, DEFAULT_PEAK_FILTERING);
}

FrankEtAlDotProduct::FrankEtAlDotProduct(float fragmentIonTolerance) {
    new(this)FrankEtAlDotProduct(fragmentIonTolerance, DEFAULT_NUMBER_OF_PEAKS_TO_COMPARE, DEFAULT_PEAK_FILTERING);
}

FrankEtAlDotProduct::FrankEtAlDotProduct(float fragmentIonTolerance, int numberOfPeaksToCompare, bool peakFiltering) {
    this->fragmentIonTolerance = fragmentIonTolerance;
    this->numberOfPeaksToCompare = numberOfPeaksToCompare;
    this->peakFiltering = peakFiltering;
}

double FrankEtAlDotProduct::assessSimilarity(IPeakMatches *peakMatches) {
    double dotProduct = 0;

    for (int i = 0; i < peakMatches->getNumberOfSharedPeaks(); i++) {
        pair<IPeak*, IPeak*> matchedPeaks = peakMatches->getPeakPair(i);

        dotProduct += convertIntensity(matchedPeaks.first) * convertIntensity(matchedPeaks.second);
    }

    // normalize the dot product
    double sumSquareIntensity1 = peakMatches->getSpectrumOne()->getSumSquareIntensity();
    double sumSquareIntensity2 = peakMatches->getSpectrumTwo()->getSumSquareIntensity();

    double denom = sqrt(sumSquareIntensity1 * sumSquareIntensity2);
    if (denom == 0)
        return 0;
    double normalizedDotProduct = dotProduct / denom;

    if (normalizedDotProduct > 1.00000001) // JAVA rounding issue
        throw("Dot product must not exceed 1. (found " + IOUtilities::FloatToString(normalizedDotProduct,"") + ")");

    if (normalizedDotProduct > 1) // fix rounding issue
        normalizedDotProduct = 1;
    pointer_pool->remove(peakMatches);//generated in getSharedPeaksAsMatches;
    return normalizedDotProduct;
}

double FrankEtAlDotProduct::assessSimilarity(ISpectrum *spectrum1, ISpectrum *spectrum2) {
    ISpectrum *highestPeaksSpectrum1, *highestPeaksSpectrum2;

    if (isPeakFiltering()) {
        // initialize the number of peaks1 to use with 15
        int numberCompared = computeNumberComparedSpectra(spectrum1, spectrum2);

        highestPeaksSpectrum1 = spectrum1->getHighestNPeaks(numberCompared);
        highestPeaksSpectrum2 = spectrum2->getHighestNPeaks(numberCompared);
    }
    else {
        // don't use peak filtering
        highestPeaksSpectrum1 = spectrum1;
        highestPeaksSpectrum2 = spectrum2;
    }

    IPeakMatches *peakMatches = PeakMatchesUtilities::getSharedPeaksAsMatches(highestPeaksSpectrum1, highestPeaksSpectrum2, (float) this->fragmentIonTolerance);

    return assessSimilarity(peakMatches);
}

double FrankEtAlDotProduct::convertIntensity(IPeak *p1) {
    double intensity = p1->getIntensity();
    if (intensity == 0)
        return 0;
    return 1 + log(intensity);
}

int FrankEtAlDotProduct::computeNumberComparedSpectra(ISpectrum *spectrum1, ISpectrum *spectrum2) {
    int numberComparedPeaks = numberOfPeaksToCompare;
    float precursorMz = spectrum1->getPrecursorMz();
    float precursor2 = spectrum2->getPrecursorMz();
    AlgorithmVersion version = AlgorithmVersion::NAT_METH_2011;
    switch (version) {
        case NAT_METH_2011:
            numberComparedPeaks = calculateK2011(precursorMz, precursor2);
            break;
        case JPR_2008:
            numberComparedPeaks = calculateK2008(precursorMz, precursor2, spectrum1->getPrecursorCharge(), spectrum2->getPrecursorCharge());
            break;
    }
    return numberComparedPeaks;
}


int FrankEtAlDotProduct::calculateK2008(float precursor1, float precursor2, int charge1, int charge2) {
    // if any of the required values is missing, return 15
    if (precursor1 == 0 || precursor2 == 0 || charge1 == 0 || charge2 == 0 || charge1 <= 0 || charge2 <= 0)
        return numberOfPeaksToCompare;

    // take 15 peaks / 1000Da peptide mass
    double peptideMass = (precursor1 * charge1 + precursor2 * charge2) / 2;

    int largeBinningRegion = numberOfPeaksToCompare;
    int k = numberOfPeaksToCompare * (int) (peptideMass / largeBinningRegion);

    if ((int)peptideMass % largeBinningRegion > 0)
        k += numberOfPeaksToCompare;

    return k;
}

int FrankEtAlDotProduct::calculateK2011(float precursor1, float precursor2) {
    // if any of the required values is missing, return 15
    if (precursor1 == 0 || precursor2 == 0)
        return numberOfPeaksToCompare;

    // use m/z / 50

    return (int) ((precursor1 / K2011_BIN_SIZE + precursor2 / K2011_BIN_SIZE) / 2);
}

string FrankEtAlDotProduct::getAlgorithmName() {
    return algrithmName;
}

float FrankEtAlDotProduct::getFragmentIonTolerance() {
    return fragmentIonTolerance;
}

bool FrankEtAlDotProduct::isPeakFiltering() {
    return peakFiltering;
}

void FrankEtAlDotProduct::setFragmentIonTolerance(float fragmentIonTolerance) {
    this->fragmentIonTolerance = fragmentIonTolerance;
}

void FrankEtAlDotProduct::setPeakFiltering(bool peakFiltering) {
    this->peakFiltering = peakFiltering;
}
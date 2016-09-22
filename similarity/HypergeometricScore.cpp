//
// Created by SangZhe on 2016/9/22.
//

#include "HypergeometricScore.h"
#include "PeakMatchesUtilities.h"

string HypergeometricScore::algorithmName = "Hypergeometric Exact Test";
  string HypergeometricScore::algorithmVersion = "0.1";
  bool HypergeometricScore::DEFAULT_PEAK_FILTERING = false;

HypergeometricScore::HypergeometricScore() {
    new(this) HypergeometricScore(Defaults::getFragmentIonTolerance(), DEFAULT_PEAK_FILTERING);
}
HypergeometricScore::HypergeometricScore(float fragmentIonTolerance) {
    this->fragmentIonTolerance = fragmentIonTolerance;
    this->peakFiltering = DEFAULT_PEAK_FILTERING;
}

HypergeometricScore::HypergeometricScore(float fragmentIonTolerance, bool peakFiltering) {
    this->fragmentIonTolerance = fragmentIonTolerance;
    this->peakFiltering = peakFiltering;
}

double HypergeometricScore::assessSimilarity(IPeakMatches *peakMatches) {
    // if there are no shared peaks, return 0 to indicate that it's random
    if (peakMatches->getNumberOfSharedPeaks() < 1)
        return 1;

    int numberOfBins = calculateNumberOfBins(peakMatches);

    return calculateSimilarityScore(peakMatches->getNumberOfSharedPeaks(),
                                    peakMatches->getSpectrumOne()->getPeaksCount(),
                                    peakMatches->getSpectrumTwo()->getPeaksCount(),
                                    numberOfBins);
}

double HypergeometricScore::assessSimilarityAsPValue(IPeakMatches *peakMatches) {
    // if there are no shared peaks, return 0 to indicate that it's random
    if (peakMatches->getNumberOfSharedPeaks() < 1)
        return 1;

    int numberOfBins = calculateNumberOfBins(peakMatches);

    return calculateSimilarityProbablity(peakMatches->getNumberOfSharedPeaks(),
                                         peakMatches->getSpectrumOne()->getPeaksCount(),
                                         peakMatches->getSpectrumTwo()->getPeaksCount(),
                                         numberOfBins);
}

int HypergeometricScore::calculateNumberOfBins(IPeakMatches *peakMatches) {
    vector<IPeak*> peaks1 = peakMatches->getSpectrumOne()->getPeaks();
    vector<IPeak*> peaks2 = peakMatches->getSpectrumTwo()->getPeaks();

    // set the maximum shared m/z value
    float minMz, maxMz; // minimum and maximum overlapping m/z

    if (peaks1[0]->getMz() < peaks2[0]->getMz()) {
        minMz = peaks1[0]->getMz();
    } else {
        minMz = peaks2[0]->getMz();
    }

    if (peaks1[peaks1.size() - 1]->getMz() > peaks2[peaks2.size() - 1]->getMz()) {
        maxMz = peaks1[peaks1.size() - 1]->getMz();
    } else {
        maxMz = peaks2[peaks2.size() - 1]->getMz();
    }

    int numberOfBins = (int)round((maxMz - minMz) / fragmentIonTolerance);

    // cannot be assessed
    if (numberOfBins < 1) {
        return 0;
    }

    if (numberOfBins < peaks1.size()|| numberOfBins < peaks2.size()) {
        return 0;
    }

    return numberOfBins;
}

double HypergeometricScore::calculateSimilarityProbablity(int numberOfSharedPeaks, int numberOfPeaksFromSpec1,
                                                          int numberOfPeaksFromSpec2, int numberOfBins) {
    if (numberOfBins < 1) {
        return 1;
    }


    boost::math::hypergeometric_distribution hyperGeometric(numberOfPeaksFromSpec1,numberOfPeaksFromSpec2,numberOfBins);


    double hgtScore = 0; // summed probability of finding more peaks
    for (int nFoundPeaks = numberOfSharedPeaks + 1; nFoundPeaks <= numberOfPeaksFromSpec2; nFoundPeaks++) {
        hgtScore += boost::math::pdf(hyperGeometric,nFoundPeaks);
    }

    if (hgtScore == 0) {
        return 1;
    }

    return hgtScore;
}

double HypergeometricScore::calculateSimilarityScore(int numberOfSharedPeaks, int numberOfPeaksFromSpec1,
                                                     int numberOfPeaksFromSpec2, int numberOfBins) {
    double pValue = calculateSimilarityProbablity(numberOfSharedPeaks, numberOfPeaksFromSpec1, numberOfPeaksFromSpec2, numberOfBins);

    return -log(pValue);
}

double HypergeometricScore::assessSimilarity(ISpectrum *spectrum1, ISpectrum *spectrum2) {
    IPeakMatches *peakMatches = PeakMatchesUtilities::getSharedPeaksAsMatches(spectrum1, spectrum2, fragmentIonTolerance, peakFiltering);
    double ret =  assessSimilarity(peakMatches);
    PointerPool::remove(peakMatches);
    return ret;
}

float HypergeometricScore::getFragmentIonTolerance() {
    return fragmentIonTolerance;
}

void HypergeometricScore::setFragmentIonTolerance(float fragmentIonTolerance) {
    this->fragmentIonTolerance = fragmentIonTolerance;
}

void HypergeometricScore::setPeakFiltering(bool peakFiltering) {
    this->peakFiltering = peakFiltering;
}

bool HypergeometricScore::isPeakFiltering() {
    return peakFiltering;
}
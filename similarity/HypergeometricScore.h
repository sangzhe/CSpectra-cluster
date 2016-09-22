//
// Created by SangZhe on 2016/9/22.
//

#ifndef CSPECTRA_CLUSTER_HYPERGEOMETRICSCORE_H
#define CSPECTRA_CLUSTER_HYPERGEOMETRICSCORE_H


#include "ISimilarityChecker.h"
#include "IPeakMatches.h"
#include "../util/Defaults.h"
#include "boost/math/distributions/hypergeometric.hpp"


class HypergeometricScore: public ISimilarityChecker {
public:
    static  string algorithmName ;
    static  string algorithmVersion ;
    static  bool DEFAULT_PEAK_FILTERING ;

    HypergeometricScore();
    HypergeometricScore(float fragmentIonTolerance);
    HypergeometricScore(float fragmentIonTolerance, bool peakFiltering);
    double assessSimilarity(IPeakMatches* peakMatches);
    double assessSimilarityAsPValue(IPeakMatches* peakMatches);
    double assessSimilarity(ISpectrum* spectrum1, ISpectrum* spectrum2);
    void setFragmentIonTolerance(float fragmentIonTolerance);
    float getFragmentIonTolerance();
    bool isPeakFiltering();
    void setPeakFiltering(bool peakFiltering);
private:
    bool peakFiltering;

protected:
    float fragmentIonTolerance;
    int calculateNumberOfBins(IPeakMatches* peakMatches);
    double calculateSimilarityProbablity(int numberOfSharedPeaks, int numberOfPeaksFromSpec1, int numberOfPeaksFromSpec2, int numberOfBins);
    double calculateSimilarityScore(int numberOfSharedPeaks, int numberOfPeaksFromSpec1, int numberOfPeaksFromSpec2, int numberOfBins);


};


#endif //CSPECTRA_CLUSTER_HYPERGEOMETRICSCORE_H

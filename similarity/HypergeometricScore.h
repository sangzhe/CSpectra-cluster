//
// Created by SangZhe on 2016/9/22.
//

#ifndef CSPECTRA_CLUSTER_HYPERGEOMETRICSCORE_H
#define CSPECTRA_CLUSTER_HYPERGEOMETRICSCORE_H


#include "ISimilarityChecker.h"
#include "boost/math/distributions/hypergeometric.hpp"
#include "../pool/PoolFactory.h"


class HypergeometricScore: public ISimilarityChecker {
public:
    static  string algorithmName ;
    static  string algorithmVersion ;
    static  bool DEFAULT_PEAK_FILTERING ;
    static float DEFAULT_FRAGMENT_ION_TOLERANCE;

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
    string getAlgorithmName();
private:
    PointerPool* pointer_pool = PoolFactory::getInstance();
    bool peakFiltering;

protected:
    float fragmentIonTolerance;
    int calculateNumberOfBins(IPeakMatches* peakMatches);
    double calculateSimilarityProbablity(int numberOfSharedPeaks, int numberOfPeaksFromSpec1, int numberOfPeaksFromSpec2, int numberOfBins);
    double calculateSimilarityScore(int numberOfSharedPeaks, int numberOfPeaksFromSpec1, int numberOfPeaksFromSpec2, int numberOfBins);


};


#endif //CSPECTRA_CLUSTER_HYPERGEOMETRICSCORE_H

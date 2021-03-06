//
// Created by SangZhe on 2016/9/26.
//

#ifndef CSPECTRA_CLUSTER_COMBINEDFISHERINTENSITYTEST_H
#define CSPECTRA_CLUSTER_COMBINEDFISHERINTENSITYTEST_H


#include <boost/math/distributions/chi_squared.hpp>
#include "FisherExactTest.h"
#include "IntensityRankCorrelation.h"



class CombinedFisherIntensityTest: public ISimilarityChecker {
public:
    static string algorithmName;
    static string algorithmVersion;

    static  bool DEFAULT_PEAK_FILTERING;
    static float DEFAULT_FRAGMENT_ION_TOLERANCE;

    CombinedFisherIntensityTest();
    CombinedFisherIntensityTest(float fragmentIonTolerance);
    CombinedFisherIntensityTest(float fragmentIonTolerance, bool peakFiltering);
    double assessSimilarity(ISpectrum* spectrum1, ISpectrum* spectrum2);
    double assessSimilarity(const PeakMatches& peakMatches);
    bool isPeakFiltering();
    void setPeakFiltering(bool peakFiltering);
    void setFragmentIonTolerance(float fragmentIonTolerance);
    float getFragmentIonTolerance();
    string getAlgorithmName();

private:
    bool peakFiltering;

protected:
    FisherExactTest fisherExactTest;
    IntensityRankCorrelation intensityRankCorrelation;
    boost::math::chi_squared chiSquaredDistribution  = boost::math::chi_squared(4);
    float fragmentIonTolerance;

};


#endif //CSPECTRA_CLUSTER_COMBINEDFISHERINTENSITYTEST_H

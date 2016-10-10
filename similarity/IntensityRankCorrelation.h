//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_INTENSITYRANKCORRELATION_H
#define CSPECTRA_CLUSTER_INTENSITYRANKCORRELATION_H


#include "ISimilarityChecker.h"
#include "PeakMatches.h"
#include "PeakMatchesUtilities.h"
#include "math.h"


class IntensityRankCorrelation: public ISimilarityChecker {
public:
    static bool DEFAULT_PEAK_FILTERING;
    static float DEFAULT_FRAGMENT_ION_TOLERANCE;



    IntensityRankCorrelation();
    IntensityRankCorrelation(float fragmentIonTolerance);
    IntensityRankCorrelation(float fragmentIonTolerance, bool peakFiltering);
    double assessSimilarityAsPValue(IPeakMatches* peakMatches);
    double assessSimilarity(IPeakMatches* peakMatches);
    double assessSimilarity( ISpectrum* spectrum1, ISpectrum* spectrum2);
    bool isPeakFiltering();
    void setPeakFiltering(bool peakFiltering);
    void setFragmentIonTolerance(float fragmentIonTolerance);
    float getFragmentIonTolerance();
    string getAlgorithmName();

protected:
    float fragmentIonTolerance;
    bool peakFiltering;

private:
    PointerPool* pointer_pool = PoolFactory::getInstance();
    vector<double> extractPeakIntensities(const vector<Peak>& peaks);
};


#endif //CSPECTRA_CLUSTER_INTENSITYRANKCORRELATION_H

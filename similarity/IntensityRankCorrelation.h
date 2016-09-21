//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_INTENSITYRANKCORRELATION_H
#define CSPECTRA_CLUSTER_INTENSITYRANKCORRELATION_H


#include "ISimilarityChecker.h"
#include "IPeakMatches.h"

class IntensityRankCorrelation: public ISimilarityChecker {
public:
    static bool DEFAULT_PEAK_FILTERING = false;

    IntensityRankCorrelation();
    IntensityRankCorrelation(float fragmentIonTolerance);
    IntensityRankCorrelation(float fragmentIonTolerance, bool peakFiltering);
    double assessSimilarityAsPValue(IPeakMatches peakMatches);
    double assessSimilarity(IPeakMatches peakMatches);
    double assessSimilarity(ISpectrum spectrum1, ISpectrum spectrum2);

    double assessSimilarity(const ISpectrum& spectrum1,const ISpectrum& spectrum2);
    bool isPeakFiltering();
    void setPeakFiltering(bool peakFiltering);
    void setFragmentIonTolerance(float fragmentIonTolerance);
    float getFragmentIonTolerance();

protected:
    float fragmentIonTolerance;
    bool peakFiltering;

};


#endif //CSPECTRA_CLUSTER_INTENSITYRANKCORRELATION_H

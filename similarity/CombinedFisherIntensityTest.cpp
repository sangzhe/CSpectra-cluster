//
// Created by SangZhe on 2016/9/26.
//

#include "CombinedFisherIntensityTest.h"
#include "boost/math/distributions/gamma.hpp"

 string CombinedFisherIntensityTest::algorithmName = "Combined FisherExact and Intensity rank test";
 string CombinedFisherIntensityTest::algorithmVersion = "0.1";

bool CombinedFisherIntensityTest::DEFAULT_PEAK_FILTERING = false;

float CombinedFisherIntensityTest::DEFAULT_FRAGMENT_ION_TOLERANCE = 0.5F;

CombinedFisherIntensityTest::CombinedFisherIntensityTest() {
    new(this)CombinedFisherIntensityTest(DEFAULT_FRAGMENT_ION_TOLERANCE);
}

CombinedFisherIntensityTest::CombinedFisherIntensityTest(float fragmentIonTolerance) {
    new(this)CombinedFisherIntensityTest(fragmentIonTolerance,DEFAULT_PEAK_FILTERING);
}

CombinedFisherIntensityTest::CombinedFisherIntensityTest(float fragmentIonTolerance, bool peakFiltering) {
    this->fragmentIonTolerance = fragmentIonTolerance;
    this->peakFiltering = peakFiltering;
}

double CombinedFisherIntensityTest::assessSimilarity(ISpectrum *spectrum1, ISpectrum *spectrum2) {
    PeakMatches peakMatches = PeakMatchesUtilities::getSharedPeaksAsMatches(spectrum1, spectrum2, fragmentIonTolerance, peakFiltering);
    double ret =  assessSimilarity(peakMatches);
    return ret;
}


double CombinedFisherIntensityTest::assessSimilarity(const PeakMatches& peakMatches) {
    double fisherExactP = fisherExactTest.assessSimilarityAsPValue(peakMatches);
    double intensityRankP = intensityRankCorrelation.assessSimilarityAsPValue(peakMatches);
    // combine the p-values using Fisher's method
    double combined = -2 * (log(fisherExactP) + log(intensityRankP));
    double pValue;

    if (combined == 0)
        return 0;

    if (isinf(combined))
        pValue = 0;
    else
        pValue = boost::math::pdf(chiSquaredDistribution,combined);
    return -log(pValue);
}

string CombinedFisherIntensityTest::getAlgorithmName() {
    return algorithmName;
}
float CombinedFisherIntensityTest::getFragmentIonTolerance() {
    return fragmentIonTolerance;
}

bool CombinedFisherIntensityTest::isPeakFiltering() {
    return peakFiltering;
}

void CombinedFisherIntensityTest::setFragmentIonTolerance(float fragmentIonTolerance) {
    this->fragmentIonTolerance = fragmentIonTolerance;
}

void CombinedFisherIntensityTest::setPeakFiltering(bool peakFiltering) {
    this->peakFiltering = peakFiltering;
}
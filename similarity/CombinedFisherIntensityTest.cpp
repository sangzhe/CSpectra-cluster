//
// Created by SangZhe on 2016/9/26.
//

#include "CombinedFisherIntensityTest.h"
#include "boost/math/distributions/gamma.hpp"

 string CombinedFisherIntensityTest::algorithmName = "Combined FisherExact and Intensity rank test";
 string CombinedFisherIntensityTest::algorithmVersion = "0.1";

bool CombinedFisherIntensityTest::DEFAULT_PEAK_FILTERING = false;

CombinedFisherIntensityTest::CombinedFisherIntensityTest() {
    new(this)CombinedFisherIntensityTest(Defaults::getFragmentIonTolerance());
}

CombinedFisherIntensityTest::CombinedFisherIntensityTest(float fragmentIonTolerance) {
    new(this)CombinedFisherIntensityTest(fragmentIonTolerance,DEFAULT_PEAK_FILTERING);
}

CombinedFisherIntensityTest::CombinedFisherIntensityTest(float fragmentIonTolerance, bool peakFiltering) {
    this->fragmentIonTolerance = fragmentIonTolerance;
    this->peakFiltering = peakFiltering;
}

double CombinedFisherIntensityTest::assessSimilarity(ISpectrum *spectrum1, ISpectrum *spectrum2) {
    IPeakMatches *peakMatches = PeakMatchesUtilities::getSharedPeaksAsMatches(spectrum1, spectrum2, fragmentIonTolerance, peakFiltering);
    double ret =  assessSimilarity(peakMatches);
    PointerPool::remove(peakMatches);
    return ret;
}


double CombinedFisherIntensityTest::assessSimilarity(IPeakMatches *peakMatches) {
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
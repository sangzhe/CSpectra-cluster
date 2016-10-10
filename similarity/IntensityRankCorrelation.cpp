//
// Created by SangZhe on 2016/9/21.
//

#include <boost/math/distributions/normal.hpp>
#include "IntensityRankCorrelation.h"
#include "KendallCorrelation.h"


bool IntensityRankCorrelation::DEFAULT_PEAK_FILTERING = false;

float IntensityRankCorrelation::DEFAULT_FRAGMENT_ION_TOLERANCE = 0.5F;

IntensityRankCorrelation::IntensityRankCorrelation() {
    new(this) IntensityRankCorrelation(DEFAULT_FRAGMENT_ION_TOLERANCE,DEFAULT_PEAK_FILTERING);
}

IntensityRankCorrelation::IntensityRankCorrelation(float fragmentIonTolerance) {
    new(this) IntensityRankCorrelation(fragmentIonTolerance,DEFAULT_PEAK_FILTERING);
}

IntensityRankCorrelation::IntensityRankCorrelation(float fragmentIonTolerance, bool peakFiltering) {
    this->fragmentIonTolerance = fragmentIonTolerance;
    this->peakFiltering = peakFiltering;
}

double IntensityRankCorrelation::assessSimilarity(ISpectrum *spectrum1, ISpectrum *spectrum2) {
    IPeakMatches *peakMatches = PeakMatchesUtilities::getSharedPeaksAsMatches(spectrum1,spectrum2,fragmentIonTolerance, peakFiltering);
    double ret =  assessSimilarity(peakMatches);
    pointer_pool->remove(peakMatches);
    return  ret;
}

double IntensityRankCorrelation::assessSimilarity(IPeakMatches *peakMatches) {
    double pValue = assessSimilarityAsPValue(peakMatches);

    return -log(pValue);
}

vector<double> IntensityRankCorrelation::extractPeakIntensities(const vector<Peak> &peaks) {
    vector<double> intensities = vector<double>(peaks.size());
    for (int i = 0; i < peaks.size(); i++) {
        intensities[i] = (double) peaks[i].getIntensity();
    }

    return intensities;
}

double IntensityRankCorrelation::assessSimilarityAsPValue(IPeakMatches *peakMatches) {
    KendallCorrelation kendallCoreelation;

    // if there are no shared peaks, return 1 to indicate that it's random
    if (peakMatches->getNumberOfSharedPeaks() < 1)
        return 1;

    // only use the intensities
    vector<double> intensitiesSpec1 = extractPeakIntensities(peakMatches->getSharedPeaksFromSpectrumOne());
    vector<double> intensitiesSpec2 = extractPeakIntensities(peakMatches->getSharedPeaksFromSpectrumTwo());

    double correlation = kendallCoreelation.correlation(intensitiesSpec1, intensitiesSpec2);

    // if the correlation cannot be calculated, assume that there is none
    if (isnan(correlation)) {
        return 1;
    }

    // convert correlation into probability using the distribution used in Peptidome
    // Normal Distribution with mean = 0 and SD^2 = 2(2k + 5)/9k(k − 1)
    double k = (double) peakMatches->getNumberOfSharedPeaks();

    // this cannot be calculated for only 1 shared peak
    if (k == 1)
        return 1;

    double sdSquare = (2 * (2 * k + 5)) / (9 * k * (k - 1) );
    double sd = sqrt(sdSquare);

    boost::math::normal normal(0, sd);
    double probability = boost::math::cdf(normal,correlation);

    return 1 - probability;
}

bool IntensityRankCorrelation::isPeakFiltering() {
    return peakFiltering;
}

void IntensityRankCorrelation::setPeakFiltering(bool peakFiltering) {
    this->peakFiltering = peakFiltering;
}

float IntensityRankCorrelation::getFragmentIonTolerance() {
    return fragmentIonTolerance;
}
void IntensityRankCorrelation::setFragmentIonTolerance(float fragmentIonTolerance) {
    this->fragmentIonTolerance = fragmentIonTolerance;
}

string IntensityRankCorrelation::getAlgorithmName() {
    return "";
}
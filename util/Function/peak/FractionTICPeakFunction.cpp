//
// Created by SangZhe on 2016/9/28.
//

#include "FractionTICPeakFunction.h"

FractionTICPeakFunction::FractionTICPeakFunction() {
    this->fractionTotalIntensity = DEFAULT_FRACTION_TOTAL_INTENSITY;
}

FractionTICPeakFunction::FractionTICPeakFunction(float percentageTotalIntensity) {
    this->fractionTotalIntensity = percentageTotalIntensity;
}
FractionTICPeakFunction::FractionTICPeakFunction(float fractionTotalIntensity, int minimumNumberOfPeaks) {
    this->fractionTotalIntensity = fractionTotalIntensity;
    this->minimumNumberOfPeaks = minimumNumberOfPeaks;
}

vector<Peak> FractionTICPeakFunction::apply( vector<Peak>& peaks) {
    // calculate the total intensity
    double totalIntensity = 0;
    for (Peak &p : peaks)
        totalIntensity += p.getIntensity();

    // sort according to intensity (descending order)
    sort(peaks.begin(),peaks.end(), Peak::cmpPeakIntensity);

    vector<Peak> filteredPeaks;
    double retainedIntensity = 0;
    int nPeaksRetained = 0;

    for (Peak& p : peaks) {
        if (retainedIntensity / totalIntensity > fractionTotalIntensity && nPeaksRetained >= minimumNumberOfPeaks)
            break;

        filteredPeaks.push_back(p);
        retainedIntensity += p.getIntensity();
        nPeaksRetained++;
    }

    return filteredPeaks;
}

float FractionTICPeakFunction::getFractionTotalIntensity() {
    return fractionTotalIntensity;
}

int FractionTICPeakFunction::getMinimumNumberOfPeaks() {
    return minimumNumberOfPeaks;
}


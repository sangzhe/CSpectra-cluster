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

vector<IPeak*> FractionTICPeakFunction::apply(const vector<IPeak *>& peaks) {
    // calculate the total intensity
    double totalIntensity = 0;
    for (IPeak *p : peaks)
        totalIntensity += p->getIntensity();

    // sort according to intensity (descending order)
    vector<IPeak*> sortedPeaks = peaks;
    sort(sortedPeaks.begin(),sortedPeaks.end(), Peak::cmpPeakIntensity);

    vector<IPeak*> filteredPeaks;
    double retainedIntensity = 0;
    int nPeaksRetained = 0;

    for (IPeak *p : sortedPeaks) {
        if (retainedIntensity / totalIntensity > fractionTotalIntensity && nPeaksRetained >= minimumNumberOfPeaks)
            break;

        filteredPeaks.push_back(p);
        pointer_pool->add(p);
        retainedIntensity += p->getIntensity();
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


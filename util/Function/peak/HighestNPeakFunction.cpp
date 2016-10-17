//
// Created by SangZhe on 16/9/7.
//

#include "HighestNPeakFunction.h"

int HighestNPeakFunction::DEFAULT_MAX_PEAKS = 100;

HighestNPeakFunction::HighestNPeakFunction() {
    new (this)HighestNPeakFunction(DEFAULT_MAX_PEAKS);
}

HighestNPeakFunction::HighestNPeakFunction(int maxPeaks) {
    this->maxPeaks = maxPeaks;
}

vector<Peak> HighestNPeakFunction::apply(const vector<Peak> &originalPeaks) {
    vector<Peak> byIntensity = originalPeaks;
    sort(byIntensity.begin(),byIntensity.end(),Peak::cmpPeakIntensity);
    vector<Peak> ret;

    vector<Peak>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        if(ret.size() >= maxPeaks){
            break;
        }else{
            ret.push_back(*iter);
        }
    }
    return ret;
}
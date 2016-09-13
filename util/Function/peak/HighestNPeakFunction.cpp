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

list<Peak> HighestNPeakFunction::apply(list<Peak> &originalPeaks) {
    list<Peak> byIntensity = originalPeaks;
    byIntensity.sort(Peak::cmpPeakIntensity);
    list<Peak> ret;

    list<Peak>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        if(ret.size() >= maxPeaks){
            break;
        }else{
            ret.push_back(*iter);
        }
    }
    originalPeaks = list<Peak>(ret);
}
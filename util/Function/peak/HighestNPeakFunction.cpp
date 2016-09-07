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

void HighestNPeakFunction::apply(list<IPeak *> &originalPeaks) {
    list<IPeak*> byIntensity = originalPeaks;
    byIntensity.sort(Peak::cmpPeakIntensity);
    list<IPeak*> ret;

    list<IPeak*>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        if(ret.size() >= maxPeaks){
            delete *iter;
        }else{
            ret.push_back(*iter);
        }
    }
    originalPeaks = ret;
}
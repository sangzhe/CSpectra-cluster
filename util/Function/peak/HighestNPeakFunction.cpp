//
// Created by SangZhe on 16/9/7.
//

#include "HighestNPeakFunction.h"
#include "../../../pool/ClusterPointerPool.h"

int HighestNPeakFunction::DEFAULT_MAX_PEAKS = 100;

HighestNPeakFunction::HighestNPeakFunction() {
    new (this)HighestNPeakFunction(DEFAULT_MAX_PEAKS);
}

HighestNPeakFunction::HighestNPeakFunction(int maxPeaks) {
    this->maxPeaks = maxPeaks;
}

list<IPeak*> HighestNPeakFunction::apply(const list<IPeak*> &originalPeaks) {
    list<IPeak*> byIntensity = originalPeaks;
    byIntensity.sort(Peak::cmpPeakIntensity);
    list<IPeak*> ret;

    list<IPeak*>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        if(ret.size() >= maxPeaks){
            break;
        }else{
            ret.push_back(*iter);
        }
    }
    PointerPool::remove(originalPeaks);
    PointerPool::add(ret);
    return ret;
}
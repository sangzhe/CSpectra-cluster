//
// Created by SangZhe on 16/9/7.
//

#include "HighestNPeakFunction.h"

int HighestNPeakFunction::DEFAULT_MAX_PEAKS = 100;

PointerPool* HighestNPeakFunction::pointer_pool = PoolFactory::getInstance();
HighestNPeakFunction::HighestNPeakFunction() {
    new (this)HighestNPeakFunction(DEFAULT_MAX_PEAKS);
}

HighestNPeakFunction::HighestNPeakFunction(int maxPeaks) {
    this->maxPeaks = maxPeaks;
}

vector<IPeak*> HighestNPeakFunction::apply(const vector<IPeak*> &originalPeaks) {
    vector<IPeak*> byIntensity = originalPeaks;
    sort(byIntensity.begin(),byIntensity.end(),Peak::cmpPeakIntensity);
    vector<IPeak*> ret;

    vector<IPeak*>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        if(ret.size() >= maxPeaks){
            break;
        }else{
            ret.push_back(*iter);
        }
    }
    pointer_pool->remove(originalPeaks);
    pointer_pool->add(ret);
    return ret;
}
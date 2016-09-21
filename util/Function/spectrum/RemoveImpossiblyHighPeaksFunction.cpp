//
// Created by SangZhe on 16/9/7.
//

#include "RemoveImpossiblyHighPeaksFunction.h"

float RemoveImpossiblyHighPeaksFunction::DEFAULT_TOLERANCE = 3.0F;


RemoveImpossiblyHighPeaksFunction::RemoveImpossiblyHighPeaksFunction(float tolerance) {
    this->tolerance = tolerance;
}

RemoveImpossiblyHighPeaksFunction::RemoveImpossiblyHighPeaksFunction() {
    new (this)RemoveImpossiblyHighPeaksFunction(DEFAULT_TOLERANCE);
}

ISpectrum* RemoveImpossiblyHighPeaksFunction::apply(const ISpectrum &o) {
     float monoisotopicMass = Mass::getMonoisotopicMass(o.getPrecursorMz(), o.getPrecursorCharge());
     float maxMass = monoisotopicMass + Mass::PROTON + tolerance;

    vector<IPeak*> filteredPeaks;
    vector<IPeak*> peak = o.getPeaks();
    vector<IPeak*>::iterator iterator1;
    for(iterator1 = peak.begin();iterator1 != peak.end();iterator1++) {
        if ((*iterator1)->getMz() > maxMass)
            continue;
        filteredPeaks.push_back(*iterator1);
    }
    PointerPool::remove(peak);
    ISpectrum* ret = new Spectrum(o,filteredPeaks,true);
    PointerPool::add(filteredPeaks);
    return ret;
}
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

Spectrum RemoveImpossiblyHighPeaksFunction::apply(Spectrum &o) {
     float monoisotopicMass = Mass::getMonoisotopicMass(o.getPrecursorMz(), o.getPrecursorCharge());
     float maxMass = monoisotopicMass + Mass::PROTON + tolerance;

    list<Peak> filteredPeaks;
    list<Peak> peak = o.getPeaks();
    list<Peak>::iterator iterator1;
    for(iterator1 = peak.begin();iterator1 != peak.end();iterator1++) {
        if (iterator1->getMz() > maxMass)
            continue;
        Peak add(*iterator1);
        filteredPeaks.push_back(add);
    }
    Spectrum filteredSpectrum(o,filteredPeaks,true);
    return filteredSpectrum;
}
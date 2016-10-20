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

Spectrum RemoveImpossiblyHighPeaksFunction::apply( ISpectrum &o) {
     float monoisotopicMass = Mass::getMonoisotopicMass(o.getPrecursorMz(), o.getPrecursorCharge());
     float maxMass = monoisotopicMass + Mass::PROTON + tolerance;

    vector<Peak> filteredPeaks;
    vector<Peak> peak = o.getPeaks();
    vector<Peak>::iterator iterator1;
    for(iterator1 = peak.begin();iterator1 != peak.end();++iterator1) {
        if ((*iterator1).getMz() > maxMass)
            continue;
        filteredPeaks.push_back(*iterator1);
    }
    Spectrum ret(o,filteredPeaks,true);
    return ret;
}
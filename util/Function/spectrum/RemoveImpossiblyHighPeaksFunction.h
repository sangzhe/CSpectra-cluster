//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_REMOVEIMPOSSIBLYHIGHPEAKSFUNCTION_H
#define CSPECTRA_CLUSTER_REMOVEIMPOSSIBLYHIGHPEAKSFUNCTION_H


#include "../../../Spectrum/ISpectrum.h"
#include "../../../Spectrum/Spectrum.h"

#include "../../../Spectrum/Mass.h"

class RemoveImpossiblyHighPeaksFunction {
public:
    static float DEFAULT_TOLERANCE;
    float tolerance;

    RemoveImpossiblyHighPeaksFunction(float tolerance);

    RemoveImpossiblyHighPeaksFunction();

    void apply(ISpectrum* o);

};


#endif //CSPECTRA_CLUSTER_REMOVEIMPOSSIBLYHIGHPEAKSFUNCTION_H

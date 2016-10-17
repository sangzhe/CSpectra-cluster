//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_REMOVEIMPOSSIBLYHIGHPEAKSFUNCTION_H
#define CSPECTRA_CLUSTER_REMOVEIMPOSSIBLYHIGHPEAKSFUNCTION_H


#include "../../../Spectrum/Spectrum.h"

#include "../../../Spectrum/Mass.h"
#include "SpectrumFunctoin.h"

class RemoveImpossiblyHighPeaksFunction:public SpectrumFunction {
public:
    static float DEFAULT_TOLERANCE;

    RemoveImpossiblyHighPeaksFunction(float tolerance);

    RemoveImpossiblyHighPeaksFunction();

    ISpectrum* apply(const ISpectrum& o);

private:
    float tolerance;

};


#endif //CSPECTRA_CLUSTER_REMOVEIMPOSSIBLYHIGHPEAKSFUNCTION_H

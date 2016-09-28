//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_REMOVEPRECURSORPEAKSFUNCTION_H
#define CSPECTRA_CLUSTER_REMOVEPRECURSORPEAKSFUNCTION_H


#include "../../../Spectrum/Mass.h"
#include "../../../Spectrum/Spectrum.h"
#include "SpectrumFunctoin.h"


class RemovePrecursorPeaksFunction:public SpectrumFunction{
private:
    float fragmentIonTolerance;



public:
    RemovePrecursorPeaksFunction(float fragmentIonTolerance);
    bool isWithinRange(float min, float max, float value);

    ISpectrum* apply(const ISpectrum& o);


};


#endif //CSPECTRA_CLUSTER_REMOVEPRECURSORPEAKSFUNCTION_H

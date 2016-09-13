//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_REMOVEPRECURSORPEAKSFUNCTION_H
#define CSPECTRA_CLUSTER_REMOVEPRECURSORPEAKSFUNCTION_H


#include "../../../Spectrum/ISpectrum.h"
#include "../../../Spectrum/Mass.h"
#include "../../../Spectrum/Spectrum.h"


class RemovePrecursorPeaksFunction {
private:
    float fragmentIonTolerance;



public:
    RemovePrecursorPeaksFunction(float fragmentIonTolerance);
    bool isWithinRange(float min, float max, float value);

    void apply(Spectrum& o);


};


#endif //CSPECTRA_CLUSTER_REMOVEPRECURSORPEAKSFUNCTION_H

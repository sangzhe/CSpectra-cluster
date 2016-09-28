//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_SPECTRUMPREDICATE_H
#define CSPECTRA_CLUSTER_SPECTRUMPREDICATE_H

#include "../../../Spectrum/ISpectrum.h"

class SpectrumPredicate{
public:
    virtual bool apply(ISpectrum* spc1,ISpectrum* spec2) = 0;
};
#endif //CSPECTRA_CLUSTER_SPECTRUMPREDICATE_H

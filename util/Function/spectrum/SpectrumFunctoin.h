//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_SPECTRUMFUNCTOIN_H
#define CSPECTRA_CLUSTER_SPECTRUMFUNCTOIN_H

#include "../../../Spectrum/ISpectrum.h"

class SpectrumFunction{
    public:
    virtual ISpectrum* apply(const ISpectrum& o) = 0;
};
#endif //CSPECTRA_CLUSTER_SPECTRUMFUNCTOIN_H

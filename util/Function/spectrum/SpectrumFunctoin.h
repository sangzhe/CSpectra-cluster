//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_SPECTRUMFUNCTOIN_H
#define CSPECTRA_CLUSTER_SPECTRUMFUNCTOIN_H

#include "../../../Spectrum/Spectrum.h"

class SpectrumFunction{
    public:
    virtual Spectrum apply( ISpectrum& o) = 0;
};
#endif //CSPECTRA_CLUSTER_SPECTRUMFUNCTOIN_H

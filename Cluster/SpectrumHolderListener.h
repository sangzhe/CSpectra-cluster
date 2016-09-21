//
// Created by SangZhe on 16/9/14.
//

#ifndef CSPECTRA_CLUSTER_SPECTRUMHOLDERLISTENER_H
#define CSPECTRA_CLUSTER_SPECTRUMHOLDERLISTENER_H

#include "../Spectrum/ISpectrum.h"
#include "ISpectrumHolder.h"
#include "../IPointer.h"

class ISpectrumHolder;

class SpectrumHolderListener:virtual public IPointer{
public:
    virtual void onSpectraAdd(ISpectrumHolder* holder,list<ISpectrum*>& added) = 0;

    virtual void onSpectraRemove(ISpectrumHolder* holder,list<ISpectrum*>& removed) = 0;
};

#endif //CSPECTRA_CLUSTER_SPECTRUMHOLDERLISTENER_H

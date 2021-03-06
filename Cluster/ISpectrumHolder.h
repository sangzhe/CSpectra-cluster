//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H
#define CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H

#include <list>
#include "math.h"
#include "../IPointer.h"
#include "../Spectrum/ISpectrum.h"
#include "SpectrumHolderListener.h"

class ISpectrumHolder:virtual public IPointer{
public:
    virtual void addSpectra( ISpectrum* merged)  = 0;
    virtual void addSpectra(  vector<ISpectrum*>& spectra)  = 0;
    virtual bool isRemovedSupported() = 0;
    virtual void removeSpectra(  ISpectrum* removed)  = 0;
    virtual void removeSpectra(  vector<ISpectrum*> &spectra)  = 0;
    virtual void addSpectrumHolderListener(SpectrumHolderListener* added) = 0;
    virtual void removeSpectrumHolderListener(SpectrumHolderListener* removed) = 0;

};
#endif //CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H

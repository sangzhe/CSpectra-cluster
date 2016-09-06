//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H
#define CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H

#include <list>
#include "ISpectrum.h"
#include "math.h"

class ISpectrumHolder{
public:
    ISpectrumHolder(){};
    virtual void addSpectra(ISpectrum& merged) = 0;
    virtual void addSpectra( list<ISpectrum*>& spectra) = 0;
    virtual bool isRemovedSupported() = 0;
    virtual void removeSpectra( ISpectrum& removed) = 0;
    virtual void removeSpectra( list<ISpectrum*> &spectra) = 0;

};
#endif //CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H

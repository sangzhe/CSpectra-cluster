//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H
#define CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H

#include <list>
#include "../Spectrum/Spectrum.h"
#include "math.h"

class ISpectrumHolder{
public:
    ISpectrumHolder(){};
    virtual void addSpectra(const Spectrum& merged)  = 0;
    virtual void addSpectra(const  list<Spectrum>& spectra)  = 0;
    virtual bool isRemovedSupported() = 0;
    virtual void removeSpectra( const Spectrum& removed)  = 0;
    virtual void removeSpectra( const list<Spectrum> &spectra)  = 0;

};
#endif //CSPECTRA_CLUSTER_ISPECTRUMHOLDER_H

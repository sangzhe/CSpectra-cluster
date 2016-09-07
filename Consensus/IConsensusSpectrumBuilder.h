//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_ICONSENSUSSPECTRUMBUILDER_H
#define CSPECTRA_CLUSTER_ICONSENSUSSPECTRUMBUILDER_H

#include "list"
#include "../Spectrum/ISpectrum.h"
#include "../Spectrum/IPeak.h"
#include "../Cluster/ISpectrumHolder.h"

class IConsensusSpectrumBuilder: public ISpectrumHolder{
    public:
    IConsensusSpectrumBuilder(){};

    virtual ISpectrum* getConsensusSpectrum() = 0;
    virtual ISpectrum* internalGetConcensusSpectrum() = 0;
    virtual void clear() = 0;
    virtual int getSpectraCount() = 0;
    virtual int getSumCharge() = 0;
    virtual double getSumPrecursorMz() = 0;
    virtual double getSumPrecursorIntensity() = 0;
//    virtual list<IPeak> getRawConsensusPeaks() = 0;

};
#endif //CSPECTRA_CLUSTER_ICONSENSUSSPECTRUMBUILDER_H

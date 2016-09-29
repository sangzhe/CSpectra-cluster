//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_ICONSENSUSSPECTRUMBUILDER_H
#define CSPECTRA_CLUSTER_ICONSENSUSSPECTRUMBUILDER_H

#include "list"
#include "../Spectrum/ISpectrum.h"
#include "../Spectrum/IPeak.h"
#include "../Cluster/ISpectrumHolder.h"
#include "../Cluster/SpectrumHolderListener.h"


class IConsensusSpectrumBuilder: public ISpectrumHolder,public SpectrumHolderListener{
    public:
    IConsensusSpectrumBuilder(){};

    virtual ISpectrum* getConsensusSpectrum() = 0;
    virtual void clear() = 0;
    virtual int getSpectraCount() const = 0;
    virtual int getSumCharge() const = 0;
    virtual double getSumPrecursorMz() const = 0;
    virtual double getSumPrecursorIntensity() const = 0;
    virtual string getMethodName() = 0;
//    virtual IConsensusSpectrumBuilder& operator=(IConsensusSpectrumBuilder& O) = 0;
//    virtual list<IPeak> getRawConsensusPeaks() = 0;

};
#endif //CSPECTRA_CLUSTER_ICONSENSUSSPECTRUMBUILDER_H

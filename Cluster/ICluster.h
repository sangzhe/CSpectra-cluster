//
// Created by SangZhe on 16/8/24.
//
#include "../Spectrum/Spectrum.h"
#include "../Consensus/IConsensusSpectrumBuilder.h"
#include "list"
#include "ISpectrumHolder.h"
#include "../util/ComparisonMatch.h"
#include "uuid/uuid.h"

#ifndef CSPECTRA_CLUSTER_ICLUSTER_H
#define CSPECTRA_CLUSTER_ICLUSTER_H

class ICluster:public ISpectrumHolder{
    public:
        ICluster(){};
        virtual string getMethodName() = 0;
        virtual string getId() = 0 ;
        virtual void setId(string id) = 0 ;
        virtual string getSpectralId() =0 ;
        virtual float getPrecursorMz() = 0 ;
        virtual int getPrecursorCharge() = 0 ;
        virtual Spectrum getConsensusSpectrum() = 0 ;
        virtual IConsensusSpectrumBuilder* getConsensusSpectrumBuilder() = 0;
        virtual list<Spectrum> getClusteredSpectra() const= 0;
        virtual int getClusteredSpectraCount() = 0;
        virtual unordered_set<string> getSpectralIds()= 0;
        virtual Properties getProperties() = 0;
        virtual string getProperty(string key) = 0;
        virtual void setProperty(string key,string value) = 0;
        virtual bool storesPeakLists() = 0;

//        virtual bool operator ==(ICluster &O) = 0;

    virtual void addSpectra(const Spectrum& merged) = 0;
    virtual void addSpectra(const list<Spectrum>& spectra) = 0;
    virtual bool isRemovedSupported() = 0;
    virtual void removeSpectra(const  Spectrum& removed) = 0;
    virtual void removeSpectra(const list<Spectrum> &spectra) = 0;


};
#endif //CSPECTRA_CLUSTER_ICLUSTER_H

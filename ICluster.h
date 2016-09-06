//
// Created by SangZhe on 16/8/24.
//
#include "ISpectrum.h"
#include "IConsensusSpectrumBuilder.h"
#include "list"
#include "set"
#include "ISpectrumHolder.h"

#ifndef CSPECTRA_CLUSTER_ICLUSTER_H
#define CSPECTRA_CLUSTER_ICLUSTER_H

class ICluster:public ISpectrumHolder{
    public:
        ICluster(){};
        virtual string getId() = 0 ;
        virtual void setId(string id) = 0 ;
        virtual string getSpectralId() =0 ;
        virtual float getPrecursorMz() = 0 ;
        virtual int getPrecursorCharge() = 0 ;
        virtual ISpectrum* getConsensusSpectrum() = 0 ;
        virtual IConsensusSpectrumBuilder* getConsensusSpecrtrumBuilder() = 0;
        virtual list<ISpectrum*> getClusterdSpectra() = 0;
        virtual int getClusteredSpectraCount() = 0;
        virtual set<string> getSpectralIds() = 0;
        virtual Properties* getProperties() = 0;
        virtual string getProperty(string key) = 0;
        virtual void setProperty(string key,string value) = 0;
        virtual bool storesPeakLists() = 0;


};
#endif //CSPECTRA_CLUSTER_ICLUSTER_H

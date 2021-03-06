//
// Created by SangZhe on 16/8/24.
//


#ifndef CSPECTRA_CLUSTER_ICLUSTER_H
#define CSPECTRA_CLUSTER_ICLUSTER_H

#include "../Consensus/IConsensusSpectrumBuilder.h"
#include "list"
#include "ISpectrumHolder.h"
#include "../util/ComparisonMatch.h"

class IConsensusSpectrumBuilder;

class ICluster:public ISpectrumHolder{
    public:
        virtual string getMethodName() = 0;
        virtual string getId() = 0 ;
        virtual void setId(string id) = 0 ;
        virtual string getSpectralId() =0 ;
        virtual float getPrecursorMz() = 0 ;
        virtual int getPrecursorCharge() = 0 ;
        virtual ISpectrum* getConsensusSpectrum() = 0 ;
        virtual IConsensusSpectrumBuilder* getConsensusSpectrumBuilder() = 0;
        virtual vector<ISpectrum*> getClusteredSpectra() = 0;
        virtual int getClusteredSpectraCount() = 0;
        virtual list<string> getSpectralIds()= 0 ;
        virtual Properties getProperties() = 0;
        virtual string getProperty(string key) = 0;
        virtual void setProperty(string key,string value) = 0;
        virtual bool storesPeakLists() = 0;
        virtual string toString() = 0;

        virtual list<ComparisonMatch> getComparisonMatches() = 0;

        virtual bool isKnownComparisonMatch(string clusterId) = 0 ;

        static bool cmp(ICluster* A, ICluster* B){
//            ToDo compare cluster: sorting
            if (A == B)
                return true;
            try {
                int ret = IOUtilities::compare(A->getPrecursorMz(), B->getPrecursorMz());
                if (ret != 0)
                    return (ret == -1);

                if (B->getClusteredSpectraCount() != A->getClusteredSpectraCount()) {
                    return (A->getClusteredSpectraCount() < B->getClusteredSpectraCount());
                }

                string spectra = A->getSpectralId();
                string otherSpectra = B->getSpectralId();
                return (spectra.compare(otherSpectra) == -1);

            } catch (exception) {
                //  give up use hash code
            }

            return true;
        }



//        virtual bool operator ==(ICluster &O) = 0;
    virtual void addSpectra(vector<ISpectrum*>& spectra) = 0;
    virtual void addSpectra(ISpectrum* merged) = 0;
    virtual bool isRemovedSupported() = 0;
    virtual void removeSpectra(ISpectrum* removed) = 0;
    virtual void removeSpectra(vector<ISpectrum*> &spectra) = 0;


};
#endif //CSPECTRA_CLUSTER_ICLUSTER_H

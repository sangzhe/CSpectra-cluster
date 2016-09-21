//
// Created by SangZhe on 16/8/24.
//


#ifndef CSPECTRA_CLUSTER_ISPECTRUM_H
#define CSPECTRA_CLUSTER_ISPECTRUM_H

#include "ISpectrumQuality.h"
#include "../Property/Properties.h"
#include "IPeak.h"

class IQualityScorer;
using namespace std;
class ISpectrum: public ISpectrumQuality{
    public:
        ISpectrum(){};
        virtual string getId() const = 0;
        virtual float getPrecursorMz()  const = 0;
        virtual int getPrecursorCharge()  const = 0;
        virtual double getTotalIntensity() const  = 0;
        virtual double getSumSquareIntensity()  const = 0;
        virtual vector<IPeak*> getPeaks()  const = 0;
        virtual int getPeaksCount()  const = 0;
    virtual ISpectrum* getHighestNPeaks(int numberRequested) = 0;
//        virtual int asMajorPeaksMZs(int majorPeakCount) = 0;
        virtual string getProperty(string key) = 0;
        virtual void setProperty(string key,string value) = 0;
        virtual Properties getProperties()  const = 0;



};
#endif //CSPECTRA_CLUSTER_ISPECTRUM_H

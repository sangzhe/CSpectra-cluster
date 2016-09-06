//
// Created by SangZhe on 16/8/24.
//


#ifndef CSPECTRA_CLUSTER_ISPECTRUM_H
#define CSPECTRA_CLUSTER_ISPECTRUM_H

#include "string"
#include "IPeak.h"
#include "list"
#include "ISpectrumQuality.h"
#include "Properties.h"

class IQualityScorer;

using namespace std;
class ISpectrum: public ISpectrumQuality{
    public:
        ISpectrum(){};
        virtual string getId() = 0;
        virtual float getPrecursorMz() = 0;
        virtual int getPrecursorCharge() = 0;
        virtual double getTotalIntensity() = 0;
        virtual double getSumSquareIntensity() = 0;
        virtual list<IPeak*> getPeaks() = 0;
        virtual int getPeaksCount() = 0;
        virtual ISpectrum* getHighestNPeaks(int numberRequested) = 0;
//        virtual int asMajorPeaksMZs(int majorPeakCount) = 0;
        virtual string getProperty(string key) = 0;
        virtual void setProperty(string key,string value) = 0;
        virtual Properties* getProperties() = 0;



};
#endif //CSPECTRA_CLUSTER_ISPECTRUM_H

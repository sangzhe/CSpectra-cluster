//
// Created by SangZhe on 16/8/25.
//

#ifndef CSPECTRA_CLUSTER_SPECTRUM_H
#define CSPECTRA_CLUSTER_SPECTRUM_H


#include <vector>
#include "ISpectrum.h"
#include "Properties.h"
#include "IQualityScorer.h"
#include "map"
#include "Peak.h"
#include "math.h"

class Spectrum: public ISpectrum{
    private:
        static int BAD_QUALITY_MEASURE;
        string id;
        int precursorCharge;
        float precursorMz;
        list<IPeak*> peaks;
        Properties* properties;

        double totalIntensity;
        double sumSquareIntensity;

        IQualityScorer* qualityScorer;
        double qualityMeasure = BAD_QUALITY_MEASURE;

        map<int,ISpectrum*> highestPeaks;
        list<int> majorPeakMZ;
        int currentMAjorPeakCount;

        void calculateIntensities();

        double convertIntensity(IPeak& p1);

        ISpectrum* buildHighestPeaks(int numberRequested);





    public:

        Spectrum( string& pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer, list<IPeak*>& inpeaks);

        Spectrum( ISpectrum& spectrum);

        Spectrum( ISpectrum& spectrum, list<IPeak*>& inpeaks);

        Spectrum( ISpectrum& spectrum, list<IPeak*>& inpeaks,bool isSortedList);

        string getId();

        double getQualityScore();

        IQualityScorer* getQualityScorer();

        float getPrecursorMz() ;
        int getPrecursorCharge() ;
        double getTotalIntensity();
        double getSumSquareIntensity();
        list<IPeak*> getPeaks();
        int getPeaksCount();
        ISpectrum* getHighestNPeaks(int numberRequested);
//        int asMajorPeaksMZs(int majorPeakCount);
        string getProperty(string key);
        void setProperty(string key,string value);
        Properties* getProperties();

        ~Spectrum();


};


#endif //CSPECTRA_CLUSTER_SPECTRUM_H

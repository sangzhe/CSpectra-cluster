//
// Created by SangZhe on 16/8/25.
//

#ifndef CSPECTRA_CLUSTER_SPECTRUM_H
#define CSPECTRA_CLUSTER_SPECTRUM_H


#include <boost/unordered/unordered_map.hpp>
#include "ISpectrum.h"
#include "../util/IOUtilities.h"
#include "../Property/Properties.h"
#include "../quality/IQualityScorer.h"
#include "math.h"
#include "../pool/ClusterPointerPool.h"
#include "Peak.h"


class Spectrum: public ISpectrum{
    private:
        static int BAD_QUALITY_MEASURE;
        string id;
        int precursorCharge;
        float precursorMz;
        vector<IPeak*> peaks;
        Properties properties;

        double totalIntensity;
        double sumSquareIntensity;

        IQualityScorer* qualityScorer;
        double qualityMeasure = BAD_QUALITY_MEASURE;

        unordered_map<int,ISpectrum*> highestPeaks;
        list<int> majorPeakMZ;
        int currentMAjorPeakCount;

        void calculateIntensities();

        double convertIntensity(IPeak& p1);

        Spectrum buildHighestPeaks(int numberRequested) const;





    public:
        Spectrum();

        Spectrum( string& pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer, const vector<IPeak*>& inpeaks);

        Spectrum(const ISpectrum& spectrum);



        Spectrum( const ISpectrum& spectrum,  const vector<IPeak*>& inpeaks);

        Spectrum( const ISpectrum& spectrum,  const vector<IPeak*>& inpeaks,bool isSortedList);

        string getId() const;

        double getQualityScore();

        IQualityScorer* getQualityScorer()  const ;

        float getPrecursorMz() const;
        int getPrecursorCharge() const;
        double getTotalIntensity() const;
        double getSumSquareIntensity() const;
        vector<IPeak*> getPeaks() const;
        int getPeaksCount() const;
        ISpectrum* getHighestNPeaks(int numberRequested);
//        int asMajorPeaksMZs(int majorPeakCount);
        string getProperty(string key);
        void setProperty(string key,string value);
        Properties getProperties() const;

//        bool operator < ( const Spectrum& O)const;

        bool operator == ( const ISpectrum& O)const;

//        Spectrum& operator=( const Spectrum& O);

//        friend size_t hash_value(const Spectrum &p);

        ~Spectrum();




};


#endif //CSPECTRA_CLUSTER_SPECTRUM_H

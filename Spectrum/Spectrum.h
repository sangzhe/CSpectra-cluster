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
#include "Peak.h"
#include "../pool/PoolFactory.h"


class Spectrum: public ISpectrum{
    private:
        PointerPool* pointer_pool = PoolFactory::getInstance();
        static int BAD_QUALITY_MEASURE;
        string id;
        int precursorCharge;
        float precursorMz;
        vector<Peak> peaks;
        Properties properties;

        double totalIntensity;
        double sumSquareIntensity;

        IQualityScorer* qualityScorer;
        double qualityMeasure = BAD_QUALITY_MEASURE;

        unordered_map<int,ISpectrum*> highestPeaks;
        list<int> majorPeakMZ;
        int currentMAjorPeakCount = 0;

        void calculateIntensities();

        double convertIntensity(Peak& p1);

        Spectrum buildHighestPeaks(int numberRequested) const;

        ISpectrum* asMajorPeaks(int majorPeakCount);

        void guaranteeMajorPeaks(int majorPeakCount);





    public:
        Spectrum();

        Spectrum( string& pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer, const vector<Peak>& inpeaks);

        Spectrum(const ISpectrum& spectrum);



        Spectrum( const ISpectrum& spectrum,  const vector<Peak>& inpeaks);

        Spectrum( const ISpectrum& spectrum,  const vector<Peak>& inpeaks,bool isSortedList);

        string getId() const;

        double getQualityScore();

        IQualityScorer* getQualityScorer()  const ;

        float getPrecursorMz() const;
        int getPrecursorCharge() const;
        double getTotalIntensity() const;
        double getSumSquareIntensity() const;
        vector<Peak> getPeaks() const;
        int getPeaksCount() const;
        ISpectrum* getHighestNPeaks(int numberRequested);
        vector<int> asMajorPeakMZs(int majorPeakCount);
        string getProperty(string key);
        void setProperty(string key,string value);
        Properties getProperties() const;

//        bool operator < ( const Spectrum& O)const;

    static bool cmpSpectrumMZ(ISpectrum* A,ISpectrum *B);
        bool operator == ( const ISpectrum& O)const;

//        Spectrum& operator=( const Spectrum& O);

//        friend size_t hash_value(const Spectrum &p);

        ~Spectrum();




};


#endif //CSPECTRA_CLUSTER_SPECTRUM_H

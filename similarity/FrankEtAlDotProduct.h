//
// Created by SangZhe on 2016/9/27.
//

#ifndef CSPECTRA_CLUSTER_FRANKETALDOTPRODUCT_H
#define CSPECTRA_CLUSTER_FRANKETALDOTPRODUCT_H


#include "ISimilarityChecker.h"
#include "../util/Defaults.h"


class FrankEtAlDotProduct : public ISimilarityChecker{
private:
    static  int K2011_BIN_SIZE ;
    bool peakFiltering;
    float fragmentIonTolerance;
    int numberOfPeaksToCompare;
    double convertIntensity(Peak& p1);
    int calculateK2008(float precursor1, float precursor2, int charge1, int charge2);
    int calculateK2011(float precursor1, float precursor2);
    string algrithmName = "FrankEtAlDotProduct";

//    AlgorithmVersion version = AlgorithmVersion::NAT_METH_2011;
//Todo : fix enum
    

public:
    static int DEFAULT_NUMBER_OF_PEAKS_TO_COMPARE ;
    static bool DEFAULT_PEAK_FILTERING;
    enum AlgorithmVersion{
        JPR_2008, NAT_METH_2011
    };

    FrankEtAlDotProduct(float fragmentIonTolerance, int numberOfPeaksToCompare, bool peakFiltering);

    FrankEtAlDotProduct(float fragmentIonTolerance);
    FrankEtAlDotProduct();
    double assessSimilarity(const PeakMatches& peakMatches);
    double assessSimilarity(ISpectrum* spectrum1, ISpectrum* spectrum2);
    bool isPeakFiltering();
    void setPeakFiltering(bool peakFiltering);
    void setFragmentIonTolerance(float fragmentIonTolerance);
    float getFragmentIonTolerance();
    string getAlgorithmName();

protected:
    int computeNumberComparedSpectra(ISpectrum* spectrum1, ISpectrum* spectrum2);



};


#endif //CSPECTRA_CLUSTER_FRANKETALDOTPRODUCT_H

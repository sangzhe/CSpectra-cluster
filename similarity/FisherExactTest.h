//
// Created by SangZhe on 2016/9/23.
//

#ifndef CSPECTRA_CLUSTER_FISHEREXACTTEST_H
#define CSPECTRA_CLUSTER_FISHEREXACTTEST_H


#include "HypergeometricScore.h"

class FisherExactTest: public HypergeometricScore {
public:
    static string algorithmName;
    static string algorithmVersion ;
    FisherExactTest():HypergeometricScore(){};
    FisherExactTest(float peakMzTolerance):HypergeometricScore(peakMzTolerance){};
    FisherExactTest(float peakMzTolerance, bool peakFiltering):HypergeometricScore(peakMzTolerance,peakFiltering){};
    string getAlgorithmName();
    double calculateSimilarityProbablity(int numberOfSharedPeaks, int numberOfPeaksFromSpec1, int numberOfPeaksFromSpec2, int numberOfBins);
};



#endif //CSPECTRA_CLUSTER_FISHEREXACTTEST_H

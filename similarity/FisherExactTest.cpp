//
// Created by SangZhe on 2016/9/23.
//

#include "FisherExactTest.h"

 string FisherExactTest::algorithmName = "Fisher Exact Test";
 string FisherExactTest::algorithmVersion = "0.1";

FisherExactTest::FisherExactTest() {
    new(this)HypergeometricScore();
}

FisherExactTest::FisherExactTest(float peakMzTolerance) {
    new(this)HypergeometricScore(peakMzTolerance);
}

FisherExactTest::FisherExactTest(float peakMzTolerance, bool peakFiltering) {
    new(this)HypergeometricScore(peakMzTolerance,peakFiltering);
}

double FisherExactTest::calculateSimilarityProbablity(int numberOfSharedPeaks, int numberOfPeaksFromSpec1,
                                                      int numberOfPeaksFromSpec2, int numberOfBins) {
    if (numberOfBins < 1) {
        return 1;
    }

    boost::math::hypergeometric_distribution<double> hyperGeometric(numberOfPeaksFromSpec1,numberOfPeaksFromSpec2,numberOfBins);

    double hgtScore = boost::math::pdf<double>(hyperGeometric,numberOfSharedPeaks);

    if (hgtScore == 0) {
        return 1;
    }

    return hgtScore;
}
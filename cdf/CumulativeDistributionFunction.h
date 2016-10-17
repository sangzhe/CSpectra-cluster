//
// Created by SangZhe on 2016/9/27.
//

#ifndef CSPECTRA_CLUSTER_CUMULATIVEDISTRIBUTIONFUNCTION_H
#define CSPECTRA_CLUSTER_CUMULATIVEDISTRIBUTIONFUNCTION_H


#include "../util/IOUtilities.h"


class CumulativeDistributionFunction {
public:
    static  string HEADER_LINE;
    CumulativeDistributionFunction(){};
    CumulativeDistributionFunction(long totalComparisons, double scoreIncrements, vector<double> proportionPeptidesBelowScore);
    static CumulativeDistributionFunction fromString(string String) throw(exception);
    double getCdfForThreshold(double threshold);
    double probability(double threshold, int nComparisons);
    bool isSaveMatch(double similarity, int nComparisons, double maximumMixtureProbability);
protected:
    int getBinForScore(double score);
    long totalComparisons = 0;
    double scoreIncrements = 0;
    vector<double> proportionPeptidesBelowScore;
};


#endif //CSPECTRA_CLUSTER_CUMULATIVEDISTRIBUTIONFUNCTION_H

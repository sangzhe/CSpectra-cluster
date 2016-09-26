//
// Created by SangZhe on 2016/9/26.
//

#ifndef CSPECTRA_CLUSTER_KENDALLCORRELATION_H
#define CSPECTRA_CLUSTER_KENDALLCORRELATION_H
#include "../util/IOUtilities.h"

class KendallCorrelation {
public:
    KendallCorrelation(){};

    double correlation(vector<double> xArray, vector<double> yArray);

private:
    static long sum(long n);
    static int compare(pair<double,double> pair1,pair<double,double> pair2);
};


#endif //CSPECTRA_CLUSTER_KENDALLCORRELATION_H

//
// Created by SangZhe on 2016/9/27.
//

#ifndef CSPECTRA_CLUSTER_CUMULATIVEDISTRIBUTIONFUNCTIONFACTORY_H
#define CSPECTRA_CLUSTER_CUMULATIVEDISTRIBUTIONFUNCTIONFACTORY_H


#include "../similarity/ISimilarityChecker.h"
#include "CumulativeDistributionFunction.h"
#include "fstream"

class CumulativeDistributionFunctionFactory {
private:
    CumulativeDistributionFunctionFactory() {}
public:
    static CumulativeDistributionFunction getCumulativeDistributionFunctionForSimilarityMetric(ISimilarityChecker* similarityCheckerClass) throw(exception);

private:
    static CumulativeDistributionFunction getCumulativeDistributionFunctionForResource(string resource) throw(exception);

};


#endif //CSPECTRA_CLUSTER_CUMULATIVEDISTRIBUTIONFUNCTIONFACTORY_H

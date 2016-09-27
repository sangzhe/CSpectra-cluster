//
// Created by SangZhe on 2016/9/27.
//

#ifndef CSPECTRA_CLUSTER_GREEDYINCREMENTALCLUSTERINGENGINE_H
#define CSPECTRA_CLUSTER_GREEDYINCREMENTALCLUSTERINGENGINE_H


#include "IIncrementalClusteringEngine.h"
#include "../Cluster/GreedySpectralCluster.h"
#include "../cdf/CumulativeDistributionFunction.h"

class GreedyIncrementalClusteringEngine: public IIncrementalClusteringEngine {
private:
    vector<GreedySpectralCluster> clusters ;
    vector<ISpectrum*> filteredConsensusSpectra;

    ISimilarityChecker* similarityChecker;
    double windowSize;
    double mixtureProbability;
    CumulativeDistributionFunction cumulativeDistributionFunction;
    IComparisonPredicate<ICluster*> clusterComparisonPredicate;

    int currentMZAsInt;
    int minNumberOfComparisons;

};


#endif //CSPECTRA_CLUSTER_GREEDYINCREMENTALCLUSTERINGENGINE_H

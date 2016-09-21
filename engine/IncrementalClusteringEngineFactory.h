//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_INCREMENTALCLUSTERINGENGINEFACTORY_H
#define CSPECTRA_CLUSTER_INCREMENTALCLUSTERINGENGINEFACTORY_H

#include "IIncrementalClusteringEngine.h"

class IncrementalClusteringEngineFactory{
public:
    IIncrementalClusteringEngine getIncrementalClusteringEngine(float windowSize) {
         ISimilarityChecker similarityChecker = Defaults::getDefaultSimilarityChecker();
         double similarityThreshold = Defaults::getSimilarityThreshold();
        return new IncrementalClusteringEngine(similarityChecker, comparator, windowSize, similarityThreshold);
    }
};
#endif //CSPECTRA_CLUSTER_INCREMENTALCLUSTERINGENGINEFACTORY_H

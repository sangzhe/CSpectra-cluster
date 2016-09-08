//
// Created by SangZhe on 16/9/8.
//

#ifndef CSPECTRA_CLUSTER_ICLUSTERINGENGINE_H
#define CSPECTRA_CLUSTER_ICLUSTERINGENGINE_H

#include "../similarity/ISimilarityChecker.h"
#include "../Cluster/ICluster.h"

class IClusteringEngine{
public:
    virtual ISimilarityChecker getSimilarityChecker() = 0;

    virtual double getSimilarityThreshold() = 0;

    virtual list<ICluster*> getClusters() = 0;

    virtual void addClusters(list<ICluster*> cluster) = 0;

    virtual void addClusters(ICluster* cluster) = 0;

    virtual bool processClusters() = 0;

    virtual int size() = 0;
};
#endif //CSPECTRA_CLUSTER_ICLUSTERINGENGINE_H

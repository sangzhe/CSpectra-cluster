//
// Created by SangZhe on 16/9/8.
//

#ifndef CSPECTRA_CLUSTER_IINCREMENTALCLUSTERINGENGINE_H
#define CSPECTRA_CLUSTER_IINCREMENTALCLUSTERINGENGINE_H

#include "IClusteringEngine.h"

class IIncrementalClusteringEngine: public IClusteringEngine{
    virtual vector<ICluster*> addClusterIncremental(ICluster& added) = 0;
};
#endif //CSPECTRA_CLUSTER_IINCREMENTALCLUSTERINGENGINE_H

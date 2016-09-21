//
// Created by SangZhe on 16/9/8.
//

#ifndef CSPECTRA_CLUSTER_IINCREMENTALCLUSTERINGENGINE_H
#define CSPECTRA_CLUSTER_IINCREMENTALCLUSTERINGENGINE_H

#include "IClusteringEngine.h"

class IIncrementalClusteringEngine: public IClusteringEngine{
    virtual list<ICluster*> addClusterIncremental(ICluster& added);
};
#endif //CSPECTRA_CLUSTER_IINCREMENTALCLUSTERINGENGINE_H

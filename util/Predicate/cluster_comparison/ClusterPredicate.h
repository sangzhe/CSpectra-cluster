//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_CLUSTERPREDICATE_H
#define CSPECTRA_CLUSTER_CLUSTERPREDICATE_H

#include "../../../Cluster/ICluster.h"

class ClusterPredicate{
public:
    virtual bool apply(ICluster* cls1,ICluster* cls2)  = 0 ;
};
#endif //CSPECTRA_CLUSTER_CLUSTERPREDICATE_H

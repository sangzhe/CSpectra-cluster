//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_ISKNOWNCOMPARISONSPREDICATE_H
#define CSPECTRA_CLUSTER_ISKNOWNCOMPARISONSPREDICATE_H


#include "ClusterPredicate.h"

class IsKnownComparisonsPredicate: public ClusterPredicate {
public:

    bool apply(ICluster* o1, ICluster* o2);
};


#endif //CSPECTRA_CLUSTER_ISKNOWNCOMPARISONSPREDICATE_H

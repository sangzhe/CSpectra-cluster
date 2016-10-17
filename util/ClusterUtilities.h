//
// Created by SangZhe on 2016/10/12.
//

#ifndef CSPECTRA_CLUSTER_CLUSTERUTILITIES_H
#define CSPECTRA_CLUSTER_CLUSTERUTILITIES_H


#include "../Cluster/SpectralClusrer.h"
#include "Defaults.h"

class ClusterUtilities {
public:
    static ICluster* asCluster(ISpectrum* spectrum);
};


#endif //CSPECTRA_CLUSTER_CLUSTERUTILITIES_H

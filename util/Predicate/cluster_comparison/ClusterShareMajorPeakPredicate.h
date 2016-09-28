//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_CLUSTERSHAREMAJORPEAKPREDICATE_H
#define CSPECTRA_CLUSTER_CLUSTERSHAREMAJORPEAKPREDICATE_H


#include "../spectrum_comparison/SpectrumPredicate.h"
#include "ClusterPredicate.h"
#include "../spectrum_comparison/ShareMajorPeaksPredicate.h"


class ClusterShareMajorPeakPredicate:public ClusterPredicate {
private:
     SpectrumPredicate* majorPeakPredicate;
public:
    ClusterShareMajorPeakPredicate();
    ClusterShareMajorPeakPredicate(int nMajorPeaks);
    bool apply(ICluster* o1, ICluster* o2);
};


#endif //CSPECTRA_CLUSTER_CLUSTERSHAREMAJORPEAKPREDICATE_H

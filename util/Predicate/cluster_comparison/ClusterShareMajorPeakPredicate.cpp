//
// Created by SangZhe on 2016/9/28.
//

#include "ClusterShareMajorPeakPredicate.h"

ClusterShareMajorPeakPredicate::ClusterShareMajorPeakPredicate() {
    this->majorPeakPredicate = new ShareMajorPeaksPredicate();
}

ClusterShareMajorPeakPredicate::ClusterShareMajorPeakPredicate(int nMajorPeaks) {
    this->majorPeakPredicate = new ShareMajorPeaksPredicate(nMajorPeaks);
}

bool ClusterShareMajorPeakPredicate::apply(ICluster *o1, ICluster *o2) {
    return majorPeakPredicate->apply(o1->getConsensusSpectrum(), o2->getConsensusSpectrum());
}
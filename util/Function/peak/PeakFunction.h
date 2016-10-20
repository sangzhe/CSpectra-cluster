//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_PEAKFUNCTION_H
#define CSPECTRA_CLUSTER_PEAKFUNCTION_H

#include <vector>
#include "../../../Spectrum/Peak.h"

class PeakFunction{
public:
    virtual vector<Peak> apply(vector<Peak> &originalPeaks) = 0;
};
#endif //CSPECTRA_CLUSTER_PEAKFUNCTION_H

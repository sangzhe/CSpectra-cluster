//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_PEAKFUNCTION_H
#define CSPECTRA_CLUSTER_PEAKFUNCTION_H

#include <vector>
#include "../../../Spectrum/IPeak.h"

class PeakFunction{
public:
    virtual vector<IPeak*> apply(const vector<IPeak*> &originalPeaks) = 0;
};
#endif //CSPECTRA_CLUSTER_PEAKFUNCTION_H

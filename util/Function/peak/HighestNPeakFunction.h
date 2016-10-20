//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_HIGHESTNPEAKFUNCTION_H
#define CSPECTRA_CLUSTER_HIGHESTNPEAKFUNCTION_H

#include "../../../Spectrum/Peak.h"
#include "PeakFunction.h"
#include "../../../pool/PoolFactory.h"

using namespace std;
class HighestNPeakFunction :public PeakFunction{
public:
    static  int DEFAULT_MAX_PEAKS;

    HighestNPeakFunction(int maxPeaks);
    HighestNPeakFunction();
    vector<Peak> apply( vector<Peak> &originalPeaks);

private:

    int maxPeaks;
};


#endif //CSPECTRA_CLUSTER_HIGHESTNPEAKFUNCTION_H

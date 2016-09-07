//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_HIGHESTNPEAKFUNCTION_H
#define CSPECTRA_CLUSTER_HIGHESTNPEAKFUNCTION_H

#include "../../../Spectrum/Peak.h"
#include "../../../Spectrum/IPeak.h"
#include "list"
using namespace std;
class HighestNPeakFunction {
public:
    static  int DEFAULT_MAX_PEAKS;

    HighestNPeakFunction(int maxPeaks);
    HighestNPeakFunction();
    void apply(list<IPeak*> &originalPeaks);

private:
    int maxPeaks;
};


#endif //CSPECTRA_CLUSTER_HIGHESTNPEAKFUNCTION_H

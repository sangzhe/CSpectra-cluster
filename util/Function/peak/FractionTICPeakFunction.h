//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_FRACTIONTICPEAKFUNCTION_H
#define CSPECTRA_CLUSTER_FRACTIONTICPEAKFUNCTION_H


#include "../../../Spectrum/Peak.h"
#include "../../../pool/PoolFactory.h"
#include "PeakFunction.h"


class FractionTICPeakFunction:public PeakFunction {
public:
    float DEFAULT_FRACTION_TOTAL_INTENSITY = 0.9F;

    FractionTICPeakFunction();

    FractionTICPeakFunction(float percentageTotalIntensity);

    FractionTICPeakFunction(float fractionTotalIntensity, int minimumNumberOfPeaks);

    vector<Peak> apply( vector<Peak>& peaks);

    float getFractionTotalIntensity();

    int getMinimumNumberOfPeaks();

private:

    float fractionTotalIntensity;

    int minimumNumberOfPeaks = 0;
};


#endif //CSPECTRA_CLUSTER_FRACTIONTICPEAKFUNCTION_H

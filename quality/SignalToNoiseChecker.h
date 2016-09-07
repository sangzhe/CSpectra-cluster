//
// Created by SangZhe on 16/9/1.
//

#ifndef CSPECTRA_CLUSTER_SIGNALTONOISECHECKER_H
#define CSPECTRA_CLUSTER_SIGNALTONOISECHECKER_H


#include "IQualityScorer.h"
#include "../Spectrum/Peak.h"


class SignalToNoiseChecker: public IQualityScorer {

private:
    static  int NUMBER_HIGH_PEAKS;
    static  string VERSION;

public:
    SignalToNoiseChecker();

    string getCurrentVersion();

    double calculateQualityScore(ISpectrum& spectrum);



};


#endif //CSPECTRA_CLUSTER_SIGNALTONOISECHECKER_H

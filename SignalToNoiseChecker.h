//
// Created by SangZhe on 16/9/1.
//

#ifndef CSPECTRA_CLUSTER_SIGNALTONOISECHECKER_H
#define CSPECTRA_CLUSTER_SIGNALTONOISECHECKER_H


#include "IQualityScorer.h"
#include "Peak.h"


class SignalToNoiseChecker: public IQualityScorer {

private:
    static const int NUMBER_HIGH_PEAKS;
    static const string VERSION;

public:
    SignalToNoiseChecker(){};
    string getName();

    string getCurrentVersion();

    double calculateQualityScore(ISpectrum& spectrum);

    ~SignalToNoiseChecker();


};


#endif //CSPECTRA_CLUSTER_SIGNALTONOISECHECKER_H

//
// Created by SangZhe on 16/8/25.
//

#ifndef CSPECTRA_CLUSTER_DEFAULTS_H
#define CSPECTRA_CLUSTER_DEFAULTS_H


#include "IQualityScorer.h"
#include "IConsensusSpectrumBuilder.h"
#include "ConsensusSpectrumBuilderFactory.h"
#include "ConsensusSpectrum.h"
#include "SignalToNoiseChecker.h"


class Defaults {
    public:
            static IQualityScorer* getDefaultQualityScorer();
            static void getDefaultPeakFilter(ISpectrum& spectrum);
            static IConsensusSpectrumBuilder* getDefaultConsensusSpectrumBuilder();
            ~Defaults();
    private:
            static IQualityScorer* defaultQualityScorer;
            static ConsensusSpectrumBuilderFactory* consensusFactory;

};


#endif //CSPECTRA_CLUSTER_DEFAULTS_H

//
// Created by SangZhe on 16/8/25.
//

#include "Defaults.h"


static ConsensusSpectrumBuilderFactory* consensusFactory = ConsensusSpectrum::FACTORY;

static IQualityScorer* defaultQualityScorer = new SignalToNoiseChecker();

IConsensusSpectrumBuilder* Defaults::getDefaultConsensusSpectrumBuilder() {
    return consensusFactory->getConsensusSpectrumBuilder();

}

IQualityScorer* Defaults::getDefaultQualityScorer() {
    return defaultQualityScorer;

}

//ToDo functions!
void Defaults::getDefaultPeakFilter(ISpectrum& spectrum) {}
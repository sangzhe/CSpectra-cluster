//
// Created by SangZhe on 16/8/25.
//

#include "Defaults.h"

ConsensusSpectrumBuilderFactory* Defaults::consensusFactory = ConsensusSpectrum::FACTORY;

IQualityScorer* Defaults::defaultQualityScorer = new SignalToNoiseChecker();

IConsensusSpectrumBuilder* Defaults::getDefaultConsensusSpectrumBuilder() {
    return consensusFactory->getConsensusSpectrumBuilder();

}

IQualityScorer* Defaults::getDefaultQualityScorer() {
    return defaultQualityScorer;

}

//ToDo functions!
void Defaults::getDefaultPeakFilter(ISpectrum& spectrum) {}

Defaults::~Defaults() {
    delete consensusFactory,defaultQualityScorer;
}
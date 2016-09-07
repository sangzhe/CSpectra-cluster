//
// Created by SangZhe on 16/8/25.
//

#include "Defaults.h"

ConsensusSpectrumBuilderFactory* Defaults::consensusFactory = new ConsensusSpectrumFactory();

IQualityScorer* Defaults::defaultQualityScorer = new SignalToNoiseChecker();

IConsensusSpectrumBuilder* Defaults::getDefaultConsensusSpectrumBuilder() {
    return consensusFactory->getConsensusSpectrumBuilder();

}

IQualityScorer* Defaults::getDefaultQualityScorer() {
    return defaultQualityScorer;

}

//ToDo functions!
void Defaults::doDefaultPeakFilter(ISpectrum* spectrum) {

}

Defaults::~Defaults() {
    delete consensusFactory,defaultQualityScorer;
}
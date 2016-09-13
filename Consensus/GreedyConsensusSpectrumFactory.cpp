//
// Created by SangZhe on 16/9/9.
//

#include "GreedyConsensusSpectrumFactory.h"

IConsensusSpectrumBuilder* GreedyConsensusSpectrumFactory::getConsensusSpectrumBuilder() {
    return new GreedyConsensusSpectrum(Defaults::getFragmentIonTolerance());
}

GreedyConsensusSpectrum* GreedyConsensusSpectrumFactory::getGreedyConsensusSpectrumBuilder() {
    return new GreedyConsensusSpectrum(Defaults::getFragmentIonTolerance());
}

IConsensusSpectrumBuilder* GreedyConsensusSpectrumFactory::getConsensusSpectrumBuilder(string id) {
    return new GreedyConsensusSpectrum(Defaults::getFragmentIonTolerance(), id);
}

GreedyConsensusSpectrum* GreedyConsensusSpectrumFactory::getGreedyConsensusSpectrumBuilder(string id) {
    return new GreedyConsensusSpectrum(Defaults::getFragmentIonTolerance(), id);
}

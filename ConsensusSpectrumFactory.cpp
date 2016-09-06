//
// Created by SangZhe on 16/9/5.
//

#include "ConsensusSpectrumFactory.h"

IConsensusSpectrumBuilder* ConsensusSpectrumFactory::getConsensusSpectrumBuilder() {
    return new ConsensusSpectrum();
}

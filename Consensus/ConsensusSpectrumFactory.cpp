//
// Created by SangZhe on 16/9/5.
//

#include "ConsensusSpectrumFactory.h"

IConsensusSpectrumBuilder* ConsensusSpectrumFactory::getConsensusSpectrumBuilder() {
    ConsensusSpectrum* ret = new ConsensusSpectrum();
    return ret;
}

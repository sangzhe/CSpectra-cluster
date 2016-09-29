//
// Created by SangZhe on 16/9/5.
//

#ifndef CSPECTRA_CLUSTER_CONSENSUSSPECTRUMFACTORY_H
#define CSPECTRA_CLUSTER_CONSENSUSSPECTRUMFACTORY_H


#include "ConsensusSpectrumBuilderFactory.h"
#include "ConsensusSpectrum.h"


class ConsensusSpectrumFactory: public ConsensusSpectrumBuilderFactory{
public:

    ConsensusSpectrumFactory(){};

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();
};


#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUMFACTORY_H

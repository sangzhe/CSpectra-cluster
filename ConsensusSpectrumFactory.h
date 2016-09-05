//
// Created by SangZhe on 16/9/5.
//

#ifndef CSPECTRA_CLUSTER_CONSENSUSSPECTRUMFACTORY_H
#define CSPECTRA_CLUSTER_CONSENSUSSPECTRUMFACTORY_H


#include "IConsensusSpectrumBuilder.h"
#include "ConsensusSpectrumBuilderFactory.h"
#include "ConsensusSpectrum.h"


class ConsensusSpectrumFactory: public ConsensusSpectrumBuilderFactory{
public:

    ConsensusSpectrumFactory(){};

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();
    ~ConsensusSpectrumFactory();
};


#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUMFACTORY_H

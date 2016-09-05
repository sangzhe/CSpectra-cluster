//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_CONSENSUSSPECTRUMBUILDERFACTORY_H
#define CSPECTRA_CLUSTER_CONSENSUSSPECTRUMBUILDERFACTORY_H

#include "IConsensusSpectrumBuilder.h"

class ConsensusSpectrumBuilderFactory{
public:
    ConsensusSpectrumBuilderFactory(){};
    virtual IConsensusSpectrumBuilder* getConsensusSpectrumBuilder() = 0;

    virtual ~ConsensusSpectrumBuilderFactory() = 0;
};
#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUMBUILDERFACTORY_H

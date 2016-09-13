//
// Created by SangZhe on 16/9/9.
//

#ifndef CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUMFACTORY_H
#define CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUMFACTORY_H


#include "GreedyConsensusSpectrum.h"
#include "../util/Defaults.h"

class GreedyConsensusSpectrum;

class GreedyConsensusSpectrumFactory :public ConsensusSpectrumBuilderFactory{
public: 
    GreedyConsensusSpectrumFactory() {};

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();

    GreedyConsensusSpectrum* getGreedyConsensusSpectrumBuilder();

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder(string id);

    GreedyConsensusSpectrum* getGreedyConsensusSpectrumBuilder(string id);
};


#endif //CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUMFACTORY_H

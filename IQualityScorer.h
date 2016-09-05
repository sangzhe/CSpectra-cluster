//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_IQUALITYSCORER_H
#define CSPECTRA_CLUSTER_IQUALITYSCORER_H

#include "ISpectrum.h"

class ISpectrum;

class IQualityScorer{
public:
    IQualityScorer(){};
    virtual double calculateQualityScore(ISpectrum& spectrum) = 0;

    virtual ~IQualityScorer() = 0;

private:
    ISpectrum* spectrum;
};
#endif //CSPECTRA_CLUSTER_IQUALITYSCORER_H

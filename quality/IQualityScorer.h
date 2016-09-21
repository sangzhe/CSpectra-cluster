//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_IQUALITYSCORER_H
#define CSPECTRA_CLUSTER_IQUALITYSCORER_H

#include "../Spectrum/Spectrum.h"

class Spectrum;

class IQualityScorer{
public:
    IQualityScorer(){};
    virtual double calculateQualityScore(const ISpectrum& spectrum) = 0;

};
#endif //CSPECTRA_CLUSTER_IQUALITYSCORER_H

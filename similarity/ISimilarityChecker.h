//
// Created by SangZhe on 16/9/8.
//

#ifndef CSPECTRA_CLUSTER_ISIMILARITYCHECKER_H
#define CSPECTRA_CLUSTER_ISIMILARITYCHECKER_H

#include "../Spectrum/ISpectrum.h"

class ISimilarityChecker{
public:
    virtual double assessSimilarity( ISpectrum* spectrum1, ISpectrum* spectrum2) = 0;
    virtual bool isPeakFiltering() = 0;
    virtual void setPeakFiltering(bool peakFiltering) = 0;
    virtual void setFragmentIonTolerance(float fragmentIonTolerance) = 0;
    virtual float getFragmentIonTolerance() = 0;
};
#endif //CSPECTRA_CLUSTER_ISIMILARITYCHECKER_H

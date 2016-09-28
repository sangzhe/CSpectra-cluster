//
// Created by SangZhe on 2016/9/28.
//

#ifndef CSPECTRA_CLUSTER_SHAREMAJORPEAKSPREDICATE_H
#define CSPECTRA_CLUSTER_SHAREMAJORPEAKSPREDICATE_H


#include "SpectrumPredicate.h"

class ShareMajorPeaksPredicate : public SpectrumPredicate{
public:
    static int DEFAULT_MAJOR_PEAKS;
    ShareMajorPeaksPredicate(int nMajorPeaks);
    ShareMajorPeaksPredicate();
    bool apply(ISpectrum* spc1,ISpectrum* spec2);
private:
    int nMajorPeaks;

};


#endif //CSPECTRA_CLUSTER_SHAREMAJORPEAKSPREDICATE_H

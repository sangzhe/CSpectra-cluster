//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_PEAKMATCHES_H
#define CSPECTRA_CLUSTER_PEAKMATCHES_H


#include "PeakMatches.h"
#include "../pool/PoolFactory.h"

class PeakMatches{
private:
    PointerPool* pointer_pool = PoolFactory::getInstance();

    ISpectrum* spectrum1 = nullptr;
    ISpectrum* spectrum2 = nullptr;
    vector<int> sharedPeakIndecesSpec1;
    vector<int> sharedPeakIndecesSpec2;
    vector<Peak> sharedPeaksSpec1;
    vector<Peak> sharedPeaksSpec2;

public:
    PeakMatches(ISpectrum* spectrum1, ISpectrum* spectrum2, vector<int> sharedPeakIndecesSpec1, vector<int> sharedPeakIndecesSpec2);
    vector<Peak> getSharedPeaksFromSpectrumOne() ;
    vector<Peak> getSharedPeaksFromSpectrumTwo() ;
    int getNumberOfSharedPeaks() const;
    pair<Peak, Peak> getPeakPair(int nIndex) const;
    ISpectrum& getSpectrumOne() const;
    ISpectrum& getSpectrumTwo() const;

};


#endif //CSPECTRA_CLUSTER_PEAKMATCHES_H

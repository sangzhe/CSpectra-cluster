//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_PEAKMATCHES_H
#define CSPECTRA_CLUSTER_PEAKMATCHES_H


#include "IPeakMatches.h"
#include "../pool/PoolFactory.h"

class PeakMatches : public IPeakMatches{
private:
    PointerPool* pointer_pool = PoolFactory::getInstance();

    ISpectrum* spectrum1;
    ISpectrum* spectrum2;
    vector<int> sharedPeakIndecesSpec1;
    vector<int> sharedPeakIndecesSpec2;
    vector<Peak> sharedPeaksSpec1;
    vector<Peak> sharedPeaksSpec2;

public:
    PeakMatches(ISpectrum* spectrum1, ISpectrum* spectrum2, vector<int> sharedPeakIndecesSpec1, vector<int> sharedPeakIndecesSpec2);
    vector<Peak> getSharedPeaksFromSpectrumOne();
    vector<Peak> getSharedPeaksFromSpectrumTwo();
    int getNumberOfSharedPeaks();
    pair<Peak, Peak> getPeakPair(int nIndex);
    ISpectrum* getSpectrumOne();
    ISpectrum* getSpectrumTwo();

    ~PeakMatches();

};


#endif //CSPECTRA_CLUSTER_PEAKMATCHES_H

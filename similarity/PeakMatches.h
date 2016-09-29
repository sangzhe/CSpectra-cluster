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
    vector<IPeak*> sharedPeaksSpec1;
    vector<IPeak*> sharedPeaksSpec2;

public:
    PeakMatches(ISpectrum* spectrum1, ISpectrum* spectrum2, vector<int> sharedPeakIndecesSpec1, vector<int> sharedPeakIndecesSpec2);
    vector<IPeak*> getSharedPeaksFromSpectrumOne();
    vector<IPeak*> getSharedPeaksFromSpectrumTwo();
    int getNumberOfSharedPeaks();
    pair<IPeak*, IPeak*> getPeakPair(int nIndex);
    ISpectrum* getSpectrumOne();
    ISpectrum* getSpectrumTwo();

    ~PeakMatches();

};


#endif //CSPECTRA_CLUSTER_PEAKMATCHES_H

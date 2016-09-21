//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_PEAKMATCHES_H
#define CSPECTRA_CLUSTER_PEAKMATCHES_H


#include "IPeakMatches.h"

class PeakMatches : public IPeakMatches{
private:
    ISpectrum* spectrum1;
    ISpectrum* spectrum2;
    list<int> sharedPeakIndecesSpec1;
    list<int> sharedPeakIndecesSpec2;
    list<IPeak*> sharedPeaksSpec1;
    list<IPeak*> sharedPeaksSpec2;

public:
    PeakMatches(ISpectrum* spectrum1, ISpectrum* spectrum2, list<int> sharedPeakIndecesSpec1, list<int> sharedPeakIndecesSpec2);
    list<IPeak*> getSharedPeaksFromSpectrumOne();
    list<IPeak*> getSharedPeaksFromSpectrumTwo();
    int getNumberOfSharedPeaks();
    pair<IPeak*, IPeak*> getPeakPair(int nIndex);
    ISpectrum* getSpectrumOne();
    ISpectrum* getSpectrumTwo();

};


#endif //CSPECTRA_CLUSTER_PEAKMATCHES_H

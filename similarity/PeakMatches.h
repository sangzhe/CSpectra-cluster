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

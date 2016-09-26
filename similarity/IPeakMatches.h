//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_IPEAKMATCHES_H
#define CSPECTRA_CLUSTER_IPEAKMATCHES_H

#include "../Spectrum/IPeak.h"
#include "../Spectrum/ISpectrum.h"

class IPeakMatches:public IPointer{
public:
    virtual vector<IPeak*> getSharedPeaksFromSpectrumOne() = 0 ;

    virtual vector<IPeak*> getSharedPeaksFromSpectrumTwo() = 0 ;

    virtual int getNumberOfSharedPeaks() = 0 ;

    virtual pair<IPeak*, IPeak*> getPeakPair(int nIndex) = 0 ;

    virtual ISpectrum* getSpectrumOne() = 0 ;


    virtual ISpectrum* getSpectrumTwo() = 0 ;


};
#endif //CSPECTRA_CLUSTER_IPEAKMATCHES_H

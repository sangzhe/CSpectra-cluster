//
// Created by SangZhe on 2016/9/21.
//

#ifndef CSPECTRA_CLUSTER_IPEAKMATCHES_H
#define CSPECTRA_CLUSTER_IPEAKMATCHES_H

#include "../Spectrum/IPeak.h"
#include "../Spectrum/ISpectrum.h"

class IPeakMatches{

    list<IPeak*> getSharedPeaksFromSpectrumOne();

    list<IPeak*> getSharedPeaksFromSpectrumTwo();

    int getNumberOfSharedPeaks();

    pair<IPeak*, IPeak*> getPeakPair(int nIndex);

    ISpectrum* getSpectrumOne();


    ISpectrum* getSpectrumTwo();
};
#endif //CSPECTRA_CLUSTER_IPEAKMATCHES_H

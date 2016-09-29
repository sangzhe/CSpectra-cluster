//
// Created by SangZhe on 2016/9/22.
//

#ifndef CSPECTRA_CLUSTER_PEAKMATCHESUTILITIES_H
#define CSPECTRA_CLUSTER_PEAKMATCHESUTILITIES_H


#include "../Spectrum/IPeak.h"
#include "../Spectrum/Peak.h"
#include "../util/IOUtilities.h"
#include "IPeakMatches.h"
#include "PeakMatches.h"
#include "../pool/PointerPool.h"



class PeakMatchesUtilities {
private:
    static PointerPool* pointer_pool;

    static IPeak *LAST_PEAK ;

protected:
    PeakMatchesUtilities(){};

public:
    static IPeakMatches* getSharedPeaksAsMatches(ISpectrum* spectrum1, ISpectrum* spectrum2,
                                                float mzTolerance, bool applyNPeaksFilter);

    static IPeakMatches* getSharedPeaksAsMatches( ISpectrum* spectrum1,  ISpectrum* spectrum2, float mzTolerance);

    static vector<vector<int>> getSharedPeaks(const ISpectrum& spectrum1,const ISpectrum& spectrum2, float mzTolerance);

    static vector<vector<int>> getSharedPeaks2(const ISpectrum& spectrum1,const ISpectrum& spectrum2, float mzTolerance);

    static int calculateNPeaks(float precursor1, float precursor2);

};


#endif //CSPECTRA_CLUSTER_PEAKMATCHESUTILITIES_H

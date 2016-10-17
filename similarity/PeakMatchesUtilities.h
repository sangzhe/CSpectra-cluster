//
// Created by SangZhe on 2016/9/22.
//

#ifndef CSPECTRA_CLUSTER_PEAKMATCHESUTILITIES_H
#define CSPECTRA_CLUSTER_PEAKMATCHESUTILITIES_H


#include "../Spectrum/Peak.h"
#include "../util/IOUtilities.h"
#include "PeakMatches.h"
#include "../pool/PointerPool.h"



class PeakMatchesUtilities {
private:

    static Peak LAST_PEAK ;

protected:
    PeakMatchesUtilities(){};

public:
    static PeakMatches getSharedPeaksAsMatches(ISpectrum* spectrum1, ISpectrum* spectrum2,
                                                float mzTolerance, bool applyNPeaksFilter);

    static PeakMatches getSharedPeaksAsMatches( ISpectrum* spectrum1,  ISpectrum* spectrum2, float mzTolerance);

    static vector<vector<int>> getSharedPeaks( ISpectrum* spectrum1, ISpectrum* spectrum2, float mzTolerance);

    static vector<vector<int>> getSharedPeaks2( ISpectrum* spectrum1, ISpectrum* spectrum2, float mzTolerance);

    static int calculateNPeaks(float precursor1, float precursor2);

};


#endif //CSPECTRA_CLUSTER_PEAKMATCHESUTILITIES_H

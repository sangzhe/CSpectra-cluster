//
// Created by SangZhe on 16/9/8.
//

#ifndef CSPECTRA_CLUSTER_BINNEDHIGHESTNPEAKFUNCTION_H
#define CSPECTRA_CLUSTER_BINNEDHIGHESTNPEAKFUNCTION_H


#include "../../MZIntensityUtilities.h"
#include "list"
#include "math.h"
#include "../../../Spectrum/Peak.h"


using namespace std;

class BinnedHighestNPeakFunction {
public:
    static int MINIMUM_BINNED_MZ;
    static int MAXIMUM_BINNED_MZ;
    static int DEFAULT_MAX_PEAKS_PER_BIN;
    static int DEFAULT_BIN_SIZE;

    BinnedHighestNPeakFunction(int maxPeaks, int binSize, int binOverlap);

    BinnedHighestNPeakFunction(int maxPeaks, int binSize);

    BinnedHighestNPeakFunction(int maxPeaks);

    BinnedHighestNPeakFunction();

    list<Peak> apply(list<Peak> &originalPeaks);


private:
    /**
 * Number of peaks per bin
 */
    int maxPeaks;
    /**
     * Bin size in m/z
     */
    int binSize;
    /**
     * Overlap between two bins (default binSize / 2)
     */
    int binOverlap;

    int handleBin(list<Peak> allpeaks, int startpeak, unordered_set<Peak> retained, double binBottom);

};


#endif //CSPECTRA_CLUSTER_BINNEDHIGHESTNPEAKFUNCTION_H

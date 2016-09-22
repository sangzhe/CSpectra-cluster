//
// Created by SangZhe on 16/9/8.
//

#include "BinnedHighestNPeakFunction.h"
#include "../../IOUtilities.h"


int BinnedHighestNPeakFunction::MINIMUM_BINNED_MZ = 0; // this otherwise leads to quite unexpected behaviour
int BinnedHighestNPeakFunction::MAXIMUM_BINNED_MZ = MZIntensityUtilities::HIGHEST_USABLE_MZ;
int BinnedHighestNPeakFunction::DEFAULT_MAX_PEAKS_PER_BIN = 8;
int BinnedHighestNPeakFunction::DEFAULT_BIN_SIZE = 100;

BinnedHighestNPeakFunction::BinnedHighestNPeakFunction() {
    new (this)BinnedHighestNPeakFunction(DEFAULT_MAX_PEAKS_PER_BIN);
}

BinnedHighestNPeakFunction::BinnedHighestNPeakFunction(int maxPeaks) {
    new (this)BinnedHighestNPeakFunction(maxPeaks,DEFAULT_BIN_SIZE);
}

BinnedHighestNPeakFunction::BinnedHighestNPeakFunction(int maxPeaks, int binSize){
    new (this)BinnedHighestNPeakFunction(maxPeaks,binSize, binSize/2);
}

BinnedHighestNPeakFunction::BinnedHighestNPeakFunction(int maxPeaks, int binSize, int binOverlap) {
    this->maxPeaks = maxPeaks;
    this->binSize = binSize;
    this->binOverlap = binOverlap;

    if (binOverlap > binSize || binOverlap == binSize)
        throw ("Bin overlap must be smaller than the bin size.");
}

vector<IPeak*> BinnedHighestNPeakFunction::apply(vector<IPeak*> &originalPeaks) {
    unordered_set<IPeak*> retained;
    int startPeak = 0;
    for(double binBottom = MAXIMUM_BINNED_MZ;binBottom < MAXIMUM_BINNED_MZ -binSize;binBottom+=(binSize - binOverlap)){
        startPeak = handleBin(originalPeaks,startPeak,retained,binBottom);
        if(startPeak > originalPeaks.size()) break;
    }
    vector<IPeak*> ret;
    unordered_set<IPeak*>::iterator iter;
    for(iter = retained.begin();iter != retained.end();iter++){
        ret.push_back(*iter);
    }

    return ret;

}

int BinnedHighestNPeakFunction::handleBin(vector<IPeak *> allpeaks, int startpeak, unordered_set<IPeak *> retained,
                                          double binBottom) {
    int startIndexNextBin = startpeak; // the index of the next bin's peak
    double binEnd = binBottom + binSize; // end of this bin
    double nextBinStartMZ = binEnd - binOverlap; // start of next bin

    int index = startpeak;
    IPeak* currentPeak = nullptr;
    vector<IPeak*> byIntensity;
    vector<IPeak*>::iterator iter;
    for(iter =allpeaks.begin();iter != allpeaks.end();iter++){
        index++;
        IPeak* nextPeak = *iter;


//        ToDo problem?
        if(currentPeak != nullptr){
            if(currentPeak->getMz() > nextPeak->getMz()){
                if(fabs(currentPeak->getMz() - nextPeak->getMz()) > 1.2 * MZIntensityUtilities::SMALL_MZ_DIFFERENCE)
                    throw("Peaks are NOT Sorted by MZ");
            }
        }


        // store all peaks that belong to this bin
        currentPeak = nextPeak;
        float currentPeakMz = currentPeak->getMz();
        // ignore if it's before this bin
        if (currentPeakMz < binBottom)
            continue;
        // store all peaks that could belong to the next bin - iteratively building up to the right index
        if (currentPeakMz < nextBinStartMZ)
            startIndexNextBin = index;
        // done with this bin if we are above the end
        if (currentPeakMz > binEnd)
            break;

        byIntensity.push_back(currentPeak); // accumulate

    }


    int numberAdded = 0;
    sort(byIntensity.begin(),byIntensity.end(),Peak::cmpPeakIntensity);

    for(iter = byIntensity.begin();iter != byIntensity.end();iter++) {
        retained.insert(*iter);
        if (++numberAdded >= maxPeaks) break;
    }

    if(startIndexNextBin >= allpeaks.size()) return IOUtilities::INT_MAX_VALUE;

    return startIndexNextBin;

}
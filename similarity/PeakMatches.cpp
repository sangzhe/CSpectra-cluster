//
// Created by SangZhe on 2016/9/21.
//

#include "PeakMatches.h"
#include "../pool/ClusterPointerPool.h"

PeakMatches::PeakMatches( ISpectrum* spectrum1,  ISpectrum* spectrum2, list<int> sharedPeakIndecesSpec1,
                         list<int> sharedPeakIndecesSpec2) {
    // make sure the number of indices have the same length
    if (sharedPeakIndecesSpec1.size() != sharedPeakIndecesSpec2.size())
        throw("SharedPeakIndices must be of same size for both spectra");

    this->spectrum1 = spectrum1;
    this->spectrum2 = spectrum2;
    this->sharedPeakIndecesSpec1 = sharedPeakIndecesSpec1;
    this->sharedPeakIndecesSpec2 = sharedPeakIndecesSpec2;
    PointerPool::add(spectrum1);
    PointerPool::add(spectrum2);
}

list<IPeak*> PeakMatches::getSharedPeaksFromSpectrumOne() {
    if(sharedPeaksSpec1.empty()){
        list<IPeak*> peaklist = spectrum1->getPeaks();
        vector<IPeak*> peaks(peaklist.begin(),peaklist.end());
        for (int aSharedPeakIndecesSpec1 : sharedPeakIndecesSpec1) {
            IPeak *sharedPeaks = peaks[aSharedPeakIndecesSpec1];
            sharedPeaksSpec1.push_back(sharedPeaks);
            PointerPool::add(sharedPeaks);
        }
    }

    return sharedPeaksSpec1;
}

list<IPeak*> PeakMatches::getSharedPeaksFromSpectrumTwo() {
    if(sharedPeaksSpec2.empty()){
        list<IPeak*> peaklist = spectrum2->getPeaks();
        vector<IPeak*> peaks(peaklist.begin(),peaklist.end());
        for (int aSharedPeakIndecesSpec2 : sharedPeakIndecesSpec2) {
            IPeak *sharedPeaks = peaks[aSharedPeakIndecesSpec2];
            sharedPeaksSpec2.push_back(sharedPeaks);
            PointerPool::add(sharedPeaks);
        }
    }

    return sharedPeaksSpec2;
}

int PeakMatches::getNumberOfSharedPeaks() {
    return sharedPeakIndecesSpec1.size();
}

pair<IPeak*, IPeak*> PeakMatches::getPeakPair(int nIndex) {
    if (nIndex < 0)
    throw("PeakPair index must be greater than 0");
    if (nIndex >= sharedPeakIndecesSpec1.size())
        throw("Request PeakPair with index '" + nIndex + "' from " + sharedPeakIndecesSpec1.size() + " matches");

    return pair<IPeak, IPeak>(spectrum1.getPeaks().get(sharedPeakIndecesSpec1.get(nIndex)),
                                  spectrum2.getPeaks().get(sharedPeakIndecesSpec2.get(nIndex)));
}
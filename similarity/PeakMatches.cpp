//
// Created by SangZhe on 2016/9/21.
//

#include "PeakMatches.h"
#include "../pool/ClusterPointerPool.h"

PeakMatches::PeakMatches( ISpectrum* spectrum1,  ISpectrum* spectrum2, vector<int> sharedPeakIndecesSpec1,
                         vector<int> sharedPeakIndecesSpec2) {
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

vector<IPeak*> PeakMatches::getSharedPeaksFromSpectrumOne() {
    if(sharedPeaksSpec1.empty()){
        vector<IPeak*> peaklist = spectrum1->getPeaks();
        vector<IPeak*> peaks(peaklist.begin(),peaklist.end());
        for (int aSharedPeakIndecesSpec1 : sharedPeakIndecesSpec1) {
            IPeak *sharedPeaks = peaks[aSharedPeakIndecesSpec1];
            sharedPeaksSpec1.push_back(sharedPeaks);
            PointerPool::add(sharedPeaks);
        }
    }

    return sharedPeaksSpec1;
}

vector<IPeak*> PeakMatches::getSharedPeaksFromSpectrumTwo() {
    if(sharedPeaksSpec2.empty()){
        vector<IPeak*> peaklist = spectrum2->getPeaks();
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

pair<IPeak*, IPeak*> PeakMatches::getPeakPair(int nIndex){
    if (nIndex < 0)
    throw("PeakPair index must be greater than 0");
    if (nIndex >= sharedPeakIndecesSpec1.size())
//        ToDo fix string add
//        throw("Request PeakPair with index '" + nIndex + "' from " + sharedPeakIndecesSpec1.size() + " matches");

    return pair<IPeak*, IPeak*>(spectrum1->getPeaks()[sharedPeakIndecesSpec1[nIndex]],
                                  spectrum2->getPeaks()[sharedPeakIndecesSpec2[nIndex]]);
}

ISpectrum* PeakMatches::getSpectrumOne() {
    PointerPool::add(spectrum1);
    return spectrum1;
}

ISpectrum* PeakMatches::getSpectrumTwo() {
    PointerPool::add(spectrum2);
    return spectrum2;
}

PeakMatches::~PeakMatches() {
    PointerPool::remove(spectrum1);
    PointerPool::remove(spectrum2);
    PointerPool::remove(sharedPeaksSpec1);
    PointerPool::remove(sharedPeaksSpec2);

}
//
// Created by SangZhe on 2016/9/21.
//

#include "PeakMatches.h"
#include "../pool/PointerPool.h"

PeakMatches::PeakMatches( ISpectrum* spectrum1,  ISpectrum* spectrum2, vector<int> sharedPeakIndecesSpec1,
                         vector<int> sharedPeakIndecesSpec2) {
    // make sure the number of indices have the same length
    if (sharedPeakIndecesSpec1.size() != sharedPeakIndecesSpec2.size())
        throw("SharedPeakIndices must be of same size for both spectra");

    this->spectrum1 = spectrum1;
    this->spectrum2 = spectrum2;
    this->sharedPeakIndecesSpec1 = sharedPeakIndecesSpec1;
    this->sharedPeakIndecesSpec2 = sharedPeakIndecesSpec2;
    pointer_pool->add(spectrum1);
    pointer_pool->add(spectrum2);
}

vector<Peak> PeakMatches::getSharedPeaksFromSpectrumOne() {
    if(sharedPeaksSpec1.empty()){
        vector<Peak> peaklist = spectrum1->getPeaks();
        for (int aSharedPeakIndecesSpec1 : sharedPeakIndecesSpec1) {
            Peak sharedPeaks = peaklist[aSharedPeakIndecesSpec1];
            sharedPeaksSpec1.push_back(sharedPeaks);
        }
    }

    return sharedPeaksSpec1;
}

vector<Peak> PeakMatches::getSharedPeaksFromSpectrumTwo() {
    if(sharedPeaksSpec2.empty()){
        vector<Peak> peaklist = spectrum2->getPeaks();
        for (int aSharedPeakIndecesSpec2 : sharedPeakIndecesSpec2) {
            Peak sharedPeaks = peaklist[aSharedPeakIndecesSpec2];
            sharedPeaksSpec2.push_back(sharedPeaks);
        }
    }

    return sharedPeaksSpec2;
}

int PeakMatches::getNumberOfSharedPeaks() const{
    return sharedPeakIndecesSpec1.size();
}

pair<Peak, Peak> PeakMatches::getPeakPair(int nIndex) const{
    if (nIndex < 0)
    throw("PeakPair index must be greater than 0");
    if (nIndex >= sharedPeakIndecesSpec1.size())
//        ToDo fix string add
//        throw("Request PeakPair with index '" + nIndex + "' from " + sharedPeakIndecesSpec1.size() + " matches");

    return pair<Peak, Peak>(spectrum1->getPeaks()[sharedPeakIndecesSpec1[nIndex]],
                                  spectrum2->getPeaks()[sharedPeakIndecesSpec2[nIndex]]);
}

ISpectrum& PeakMatches::getSpectrumOne() const{
//    pointer_pool->add(spectrum1);
    return *spectrum1;
}

ISpectrum& PeakMatches::getSpectrumTwo() const{
//    pointer_pool->add(spectrum2);
    return *spectrum2;
}


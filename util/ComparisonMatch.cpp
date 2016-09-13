//
// Created by SangZhe on 16/8/31.
//

#include "ComparisonMatch.h"

ComparisonMatch::ComparisonMatch(string& spectrumId, float similarity) {
    this->similarity = similarity;
    this->spectrumId = spectrumId;
}

string ComparisonMatch::getSpectrumId() const {
    return spectrumId;
}

float ComparisonMatch::getSimilarity() const{
    return similarity;
}

//int ComparisonMatch::compare(const ComparisonMatch& o) {
//    return IOUtilities::compare(this->similarity,o.similarity);
//}

bool ComparisonMatch::operator<(const ComparisonMatch& o) const{
    return (IOUtilities::compare(similarity,o.similarity) == -1);
}

bool ComparisonMatch::operator==(const ComparisonMatch &O) const{
    if(IOUtilities::compare(similarity,O.similarity) == 0) return true;
    else return false;
}

ComparisonMatch& ComparisonMatch::operator=(const ComparisonMatch &O) {
    similarity = O.getSimilarity();
    spectrumId = O.getSpectrumId();
}
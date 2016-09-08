//
// Created by SangZhe on 16/8/31.
//

#include "ComparisonMatch.h"

ComparisonMatch::ComparisonMatch(string& spectrumId, float similarity) {
    this->similarity = similarity;
    this->spectrumId = spectrumId;
}

string ComparisonMatch::getSpectrumId() {
    return spectrumId;
}

float ComparisonMatch::getSimilarity() {
    return similarity;
}

int ComparisonMatch::compare(ComparisonMatch& o) {
    return IOUtilities::compare(this->similarity,o.similarity);
}
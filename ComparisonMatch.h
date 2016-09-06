//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_COMPARISONMATCH_H
#define CSPECTRA_CLUSTER_COMPARISONMATCH_H
#include "string"
#include "Functions.h"

class ComparisonMatch {
public:
    ComparisonMatch(string& spectrumId,float similarity);
    string getSpectrumId();
    float getSimilarity();
    int compare(ComparisonMatch& o);

private:
    string spectrumId;
    float similarity;

};


#endif //CSPECTRA_CLUSTER_COMPARISONMATCH_H

//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_COMPARISONMATCH_H
#define CSPECTRA_CLUSTER_COMPARISONMATCH_H
#include "string"
#include "IOUtilities.h"

class ComparisonMatch {
public:
    ComparisonMatch(string& spectrumId,float similarity);
    string getSpectrumId() const;
    float getSimilarity() const;
//    int compare(const ComparisonMatch& o);

    bool operator <(const ComparisonMatch &O) const;

    bool operator ==(const ComparisonMatch &O) const;

    ComparisonMatch& operator =(const ComparisonMatch &O);
private:
    string spectrumId;
    float similarity;

};


#endif //CSPECTRA_CLUSTER_COMPARISONMATCH_H

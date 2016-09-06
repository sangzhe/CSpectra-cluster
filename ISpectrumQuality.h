//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_ISPECTRUMQUALITY_H
#define CSPECTRA_CLUSTER_ISPECTRUMQUALITY_H

class IQualityScorer;

class ISpectrumQuality{
public:
    ISpectrumQuality(){};
    virtual double getQualityScore() = 0;
    virtual IQualityScorer* getQualityScorer() = 0;

};
#endif //CSPECTRA_CLUSTER_ISPECTRUMQUALITY_H

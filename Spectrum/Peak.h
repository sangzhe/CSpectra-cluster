//
// Created by SangZhe on 16/8/30.
//

#ifndef CSPECTRA_CLUSTER_PEAK_H
#define CSPECTRA_CLUSTER_PEAK_H

#include "IPeak.h"
#include "string"
using namespace std;

class Peak: public IPeak{
private:
    float massChargeRatio;
    float intensity;
    int count;

public:
    Peak(const float& massChargeRatio, const float& intensity);

    Peak(const float& massChargeRatio, const float& intensity,int count);

    Peak(IPeak& copied);

    float getMz();

    float getIntensity();

    int getCount();

    static bool cmpPeak( IPeak* A,  IPeak* B);

    static bool cmpPeakMz( IPeak* A,  IPeak* B);

    static bool cmpPeakIntensity( IPeak* A,  IPeak* B);

    string toString();

    bool operator == ( IPeak& O);

    ~Peak();


};


#endif //CSPECTRA_CLUSTER_PEAK_H

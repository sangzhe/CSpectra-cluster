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
    Peak(){};
    Peak(const float& massChargeRatio, const float& intensity);

    Peak(const float& massChargeRatio, const float& intensity,int count);


    Peak(const IPeak& copied);


    float getMz()const ;

    float getIntensity() const ;

    int getCount() const ;

    static int cmpPeak( IPeak* A,  IPeak* B);

    static int cmpPeakMz( IPeak* A,  IPeak* B);

    static int cmpPeakIntensity( IPeak* A,  IPeak* B);

    string toString();

    bool operator == (const IPeak& O) ;

    ~Peak(){};
//    bool operator < (const IPeak& O) const;

//    friend size_t hash_value(const IPeak &p) ;
//
//    Peak& operator =(const IPeak& O);

};


#endif //CSPECTRA_CLUSTER_PEAK_H

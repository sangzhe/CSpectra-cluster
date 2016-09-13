//
// Created by SangZhe on 16/8/30.
//

#ifndef CSPECTRA_CLUSTER_PEAK_H
#define CSPECTRA_CLUSTER_PEAK_H

#include "IPeak.h"
#include "string"
#include <boost/unordered/unordered_set.hpp>

using namespace boost::unordered;
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


    Peak(const Peak& copied);


    float getMz()const ;

    float getIntensity() const ;

    int getCount() const ;

    static int cmpPeak( Peak& A,  Peak& B);

    static int cmpPeakMz( Peak& A,  Peak& B);

    static int cmpPeakIntensity( Peak& A,  Peak& B);

    string toString();

    bool operator == (const Peak& O) const;

    bool operator < (const Peak& O) const;

    friend size_t hash_value(const Peak &p) ;
//
    Peak& operator =(const Peak& O);

};


#endif //CSPECTRA_CLUSTER_PEAK_H

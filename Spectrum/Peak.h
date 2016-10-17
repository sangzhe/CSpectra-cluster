//
// Created by SangZhe on 16/8/30.
//

#ifndef CSPECTRA_CLUSTER_PEAK_H
#define CSPECTRA_CLUSTER_PEAK_H

#include "string"

using namespace std;

class Peak{
private:
    float massChargeRatio = 0;
    float intensity = 0;
    int count = 0;
    bool empty = false;

public:
    Peak();
    Peak(const float& massChargeRatio, const float& intensity);

    Peak(const float& massChargeRatio, const float& intensity,int count);


    Peak(const Peak& copied);


    float getMz()const ;

    float getIntensity() const ;

    int getCount() const ;

    bool isEmpty() const;
    static bool cmpPeak( const Peak& A,  const Peak& B);

    static bool cmpPeakMz( const Peak& A,  const Peak& B);

    static bool cmpPeakIntensity( const Peak& A,  const Peak& B);

    string toString();

    bool operator == (const Peak& O) const;

    ~Peak(){};
    bool operator < (const Peak& O) const;

    friend size_t hash_value(const Peak &p)  ;
//
    Peak& operator =(const Peak& O) ;

};


#endif //CSPECTRA_CLUSTER_PEAK_H

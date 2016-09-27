//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_IPEAK_H
#define CSPECTRA_CLUSTER_IPEAK_H

#include "../IPointer.h"

class IPeak: public IPointer{
    public:
        IPeak(){};
        virtual float getMz() const = 0;
        virtual float getIntensity() const  = 0;
        virtual int getCount() const = 0;
        virtual bool operator == (const IPeak& O) = 0;
        virtual ~IPeak()= 0;


};
#endif //CSPECTRA_CLUSTER_IPEAK_H

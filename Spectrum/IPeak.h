//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_IPEAK_H
#define CSPECTRA_CLUSTER_IPEAK_H

class IPeak{
    public:
        IPeak(){};
        virtual float getMz() const = 0;
        virtual float getIntensity() const  = 0;
        virtual int getCount() const = 0;

};
#endif //CSPECTRA_CLUSTER_IPEAK_H

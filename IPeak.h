//
// Created by SangZhe on 16/8/24.
//

#ifndef CSPECTRA_CLUSTER_IPEAK_H
#define CSPECTRA_CLUSTER_IPEAK_H

class IPeak{
    public:
        IPeak(){};
        virtual float getMz() = 0;
        virtual float getIntensity() = 0;
        virtual int getCount() = 0;

        virtual ~IPeak() = 0 ;
};
#endif //CSPECTRA_CLUSTER_IPEAK_H

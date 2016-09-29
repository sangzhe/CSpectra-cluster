//
// Created by SangZhe on 2016/9/29.
//

#ifndef CSPECTRA_CLUSTER_POOLFACTORY_H
#define CSPECTRA_CLUSTER_POOLFACTORY_H
#include "PointerPool.h"

class PoolFactory{
private:
    static PointerPool *p;
public:
    static PointerPool* getInstance();
};
#endif //CSPECTRA_CLUSTER_POOLFACTORY_H

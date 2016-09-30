//
// Created by SangZhe on 2016/9/29.
//

#include "PoolFactory.h"

PointerPool* PoolFactory::p = nullptr;

PointerPool* PoolFactory::getInstance(){
    if (nullptr == p)
    {
        p = new PointerPool();
    }
    return p;
}
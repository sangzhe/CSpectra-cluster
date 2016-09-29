//
// Created by SangZhe on 2016/9/29.
//

#include "PoolFactory.h"

PointerPool* PoolFactory::p = new PointerPool();

PointerPool* PoolFactory::getInstance(){
    return p;
}
//
// Created by SangZhe on 2016/9/29.
//

#include "IPointer.h"

IPointer::IPointer() {
    char str[36];
    uuid_t uu;
    uuid_generate_random(uu);
    stringstream ss;
    ss << uu;
    ss >>uuid;
//    uuid = string(str);
}

string IPointer::getUUID() {
    return uuid;
}
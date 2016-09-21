//
// Created by SangZhe on 16/9/14.
//

#ifndef CSPECTRA_CLUSTER_IPOINTER_H
#define CSPECTRA_CLUSTER_IPOINTER_H

#include "string"
#include <uuid/uuid.h>

using namespace std;
class IPointer{
public:
    IPointer(){
        char str[36];
        uuid_t uu;
        uuid_generate_random(uu);
        uuid_parse(str,uu);
        uuid = string(str);
    }

    string getUUID(){
        return uuid;
    }

private:
    string uuid;


};
#endif //CSPECTRA_CLUSTER_IPOINTER_H

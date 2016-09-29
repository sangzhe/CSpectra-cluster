//
// Created by SangZhe on 16/9/14.
//

#ifndef CSPECTRA_CLUSTER_IPOINTER_H
#define CSPECTRA_CLUSTER_IPOINTER_H

#include "string"
#include <uuid/uuid.h>
#include "sstream"


using namespace std;
class IPointer{
public:
    IPointer();

    string getUUID();

private:
    string uuid;


};
#endif //CSPECTRA_CLUSTER_IPOINTER_H

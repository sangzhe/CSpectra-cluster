//
// Created by SangZhe on 16/9/14.
//

#ifndef CSPECTRA_CLUSTER_CLUSTERPAINTERPAAL_H
#define CSPECTRA_CLUSTER_CLUSTERPAINTERPAAL_H

#include "../IPointer.h"
#include "../Cluster/ICluster.h"
#include <boost/unordered/unordered_set.hpp>
#include <boost/unordered/unordered_map.hpp>

class ICluster;
using namespace boost::unordered;

class PointerPool {
private:

    boost::unordered::unordered_map<string,int> counter;

    unordered_set<string> p;
public:

    void add(IPointer* A);

    void add(const vector<ICluster*>& clusters);



    void add(const vector<ISpectrum*>& spectra);

    void remove(IPointer *R);


    void remove(const vector<ISpectrum*>& spectra);

    void remove(const vector<ICluster*>& clusters);

    int size();

    void info();

};


#endif //CSPECTRA_CLUSTER_CLUSTERPAINTERPAAL_H

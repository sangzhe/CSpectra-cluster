//
// Created by SangZhe on 16/9/14.
//

#ifndef CSPECTRA_CLUSTER_CLUSTERPAINTERPAAL_H
#define CSPECTRA_CLUSTER_CLUSTERPAINTERPAAL_H

#include "../IPointer.h"
#include "../Spectrum/IPeak.h"
#include "../Spectrum/ISpectrum.h"
#include "../Cluster/ICluster.h"
#include <boost/unordered/unordered_set.hpp>
#include <boost/unordered/unordered_map.hpp>

using namespace boost::unordered;

namespace PointerPool {

    static boost::unordered::unordered_map<string,int> counter;

    static unordered_set<IPointer*> p;

    static void add(IPointer* A){
        
        unordered_set<IPointer*>::iterator iter(find(p.begin(),p.end(),A));
        if(iter != p.end()){
            int num = counter[A->getUUID()];
            num++;
            counter[A->getUUID()] = num;
        }else{
            p.insert(A);
            counter[A->getUUID()] = 1;
        }
    }

    static void add(const vector<ICluster*>& clusters){
        for(ICluster* cluster: clusters){
            add(cluster);
        }
    }
    static void add(const vector<IPeak*>& peaks){
        for(IPeak* peak:peaks){
            add(peak);
        }
    }

    static void add(const vector<ISpectrum*>& spectra){
        for(ISpectrum *spectrum:spectra){
            add(spectra);
        }
    }

    static void remove(IPointer *R){
        unordered_set<IPointer*>::iterator iter(find(p.begin(),p.end(),R));
        if(iter != p.end()){
            int num = counter[R->getUUID()];
            num--;
            if(counter[R->getUUID()] == 0){
                p.erase(R);
                counter.erase(R->getUUID());
                delete R;
                R = nullptr;
            }else{
                counter[R->getUUID()] = num;
            }
        }else{
            return;
        }
    }

    static void remove(const vector<IPeak*>& peaks){
        for(IPeak* peak:peaks){
            remove(peak);
        }
    }

    static void remove(const vector<ISpectrum*>& spectra){
        for(ISpectrum* spectrum:spectra){
            remove(spectrum);
        }
    }
    static void remove(const vector<ICluster*>& clusters){
        for(ICluster* cluster: clusters){
            remove(cluster);
        }
    }

};


#endif //CSPECTRA_CLUSTER_CLUSTERPAINTERPAAL_H

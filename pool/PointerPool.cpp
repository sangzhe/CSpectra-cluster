//
// Created by SangZhe on 2016/9/29.
//

#include "PointerPool.h"


void PointerPool::add(IPointer* A){

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

void PointerPool::add(const vector<ICluster*>& clusters){
    for(ICluster* cluster: clusters){
        PointerPool::add((IPointer*)cluster);
    }
}
void PointerPool::add(const vector<IPeak*>& peaks){
    for(IPeak* peak:peaks){
        add(peak);
    }
}

void PointerPool::add(const vector<ISpectrum*>& spectra){
    for(ISpectrum *spectrum:spectra){
        add(spectra);
    }
}

void PointerPool::remove(IPointer *R){
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

void PointerPool::remove(const vector<IPeak*>& peaks){
    for(IPeak* peak:peaks){
        remove(peak);
    }
}

void PointerPool::remove(const vector<ISpectrum*>& spectra){
    for(ISpectrum* spectrum:spectra){
        remove(spectrum);
    }
}
void PointerPool::remove(const vector<ICluster*>& clusters){
    for(ICluster* cluster: clusters){
        PointerPool::remove((IPointer*)cluster);
    }
}

int PointerPool::size() {
    return p.size();
}
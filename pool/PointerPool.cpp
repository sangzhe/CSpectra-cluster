//
// Created by SangZhe on 2016/9/29.
//

#include <iostream>
#include "PointerPool.h"


void PointerPool::add(IPointer* A){
    try {
        unordered_set<string>::iterator iter(find(p.begin(), p.end(), A->getUUID()));
        if (iter != p.end()) {
            int num = counter[A->getUUID()];
            num++;
            counter[A->getUUID()] = num;
//        std::cout<< "add "<< A->getUUID() <<endl;


        } else {
            p.insert(A->getUUID());
            counter[A->getUUID()] = 1;
//        std::cout<< "add reference"<< A->getUUID() <<endl;

        }
    }catch(exception e){
        cout<< e.what() <<endl;
    }
}

void PointerPool::add(const vector<ICluster*>& clusters){
    vector<ICluster*> inClusters = clusters;
    for(ICluster* cluster:inClusters){
        add(cluster);
    }
}

void PointerPool::add(const vector<ISpectrum*>& spectra) {
    vector<ISpectrum*> inSpectra = spectra;
    for(ISpectrum* spectrum:inSpectra){
        add(spectrum);
    }
}

void PointerPool::remove(IPointer *R){
    unordered_set<string>::iterator iter(find(p.begin(),p.end(),R->getUUID()));
    if(iter != p.end()){
        int num = counter[R->getUUID()];
        num--;
        if(num == 0){
            p.erase(R->getUUID());
            counter.erase(R->getUUID());
            std::cout<< "delete "<< R->getUUID() <<endl;
            delete R;
            R = nullptr;
        }else{
            counter[R->getUUID()] = num;
        }
    }else{
        return;
    }
}



void PointerPool::remove(const vector<ISpectrum*>& spectra){
    vector<ISpectrum*> inSpectra = spectra;
    for(ISpectrum* spectrum:inSpectra){
        remove(spectrum);
    }
}
void PointerPool::remove(const vector<ICluster*>& clusters){
    vector<ICluster*> inClusters = clusters;
    for(ICluster* cluster: inClusters){
        PointerPool::remove((IPointer*)cluster);
    }
}

int PointerPool::size() {
    return p.size();
}

void PointerPool::info() {
    for(pair<string,int> x:counter){
        std::cout<< x.first <<"\t------------\t"<<x.second<<endl;
    }
}
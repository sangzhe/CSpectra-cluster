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
    for(ICluster* cluster: clusters){
        PointerPool::add((IPointer*)cluster);
    }
}

void PointerPool::add(const vector<ISpectrum*>& spectra){
    try {
        for (ISpectrum *spectrum:spectra) {
            add(spectra);
        }
    }catch(exception e){
        cout<<e.what()<<endl;
    }

}

void PointerPool::remove(IPointer *R){
    unordered_set<string>::iterator iter(find(p.begin(),p.end(),R->getUUID()));
    if(iter != p.end()){
        int num = counter[R->getUUID()];
        num--;
        if(counter[R->getUUID()] == 0){
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

void PointerPool::info() {
    for(pair<string,int> x:counter){
        std::cout<< x.first <<"\t------------\t"<<x.second<<endl;
    }
}
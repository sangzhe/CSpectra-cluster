//
// Created by SangZhe on 16/8/31.
//

#include "SpectralClusrer.h"

string SpectralCluster::MethodName = "SpectralCluster";


SpectralCluster::SpectralCluster(ICluster &copied, IConsensusSpectrumBuilder &consensusSpectrumBuilder) {
    new (this)SpectralCluster(copied.getId(),consensusSpectrumBuilder);
    if(!copied.storesPeakLists()) throw("Clusters can only be copied if peak lists are stored.");

    list<Spectrum> clusterdSpectra1 = copied.getClusteredSpectra();
    addSpectra(clusterdSpectra1);
}

SpectralCluster::SpectralCluster(string id, IConsensusSpectrumBuilder& consensusSpectrumBuilder) {
    this->id = id;
    this->consensusSpectrumBuilder = &consensusSpectrumBuilder;
}

unordered_set<string> SpectralCluster::getSpectralIds() {
    if(this->spectraIds.empty()){
       list<Spectrum> clusteredSpectra1 = getClusteredSpectra();
        list<Spectrum>::iterator iter;
        for(iter = clusteredSpectra1.begin();iter != clusteredSpectra1.end();iter++){
            spectraIds.insert(iter->getId());
        }
        return spectraIds;
    }
}

string SpectralCluster::getMethodName() {
    return MethodName;
}

string SpectralCluster::getSpectralId() {
    string sb;
    unordered_set<string> spectraIds = getSpectralIds();
    if(spectraIds.size() > 1){
        unordered_set<string>::iterator iter = spectraIds.begin();
        sb.append(*iter);
        iter++;
        for(iter;iter != spectraIds.end();iter++){
            sb.append(",");
            sb.append(*iter);
        }
        return sb;
    }
    else {
        return *spectraIds.begin();
    }
}
string SpectralCluster::getId() {
    if (id == "") id = getSpectralId();
    return id;
}

void SpectralCluster::setId(string id) {
    this->id = id;
}

float SpectralCluster::getPrecursorMz() {
    Spectrum consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == Spectrum()) return 0;
    return consensusSpecrum1.getPrecursorMz();
}

int SpectralCluster::getPrecursorCharge() {
    Spectrum consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == Spectrum()) return 0;
    return consensusSpecrum1.getPrecursorCharge();
}



Spectrum SpectralCluster::getConsensusSpectrum() {
    if(clusteredSpectra.size() ==1) return (*(clusteredSpectra.begin()));
    return consensusSpectrumBuilder->getConsensusSpectrum();
}

list<Spectrum> SpectralCluster::getClusteredSpectra() {
    return clusteredSpectra;
}

IConsensusSpectrumBuilder* SpectralCluster::getConsensusSpectrumBuilder() {
    return consensusSpectrumBuilder;
}

int SpectralCluster::getClusteredSpectraCount() {
    return clusteredSpectra.size();
}

void SpectralCluster::addSpectra(const Spectrum &merged) {
    Spectrum added = merged;
    string Id = added.getId();
    spectraIds.insert(Id);
    list<Spectrum>::iterator iterator1;
    if(clusteredSpectra.size() == 0){
        clusteredSpectra.push_back(added);
    }else {
        int n =0;
        int range = clusteredSpectra.size();
        for (iterator1 = clusteredSpectra.begin(); n < range; iterator1++) {
            n++;
            if (! ((*iterator1) == merged))
                clusteredSpectra.push_back(added);break;
        }
    }
//    if (*find(clusteredSpectra.begin(), clusteredSpectra.end(), added)) {
//        clusteredSpectra.push_back(added);
//    }
}

void SpectralCluster::addSpectra(const list<Spectrum> &spectra) {
    if( spectra.size() > 0){
        list<Spectrum> in = spectra;
        list<Spectrum>::iterator iter;
        for(iter = in.begin();iter != in.end();iter++){
            addSpectra(*iter);
            }
    }
}

void SpectralCluster::removeSpectra(const list<Spectrum> &spectra) {
    if (!isRemovedSupported())
        throw ("Remove not supported");
    if (spectra.size() > 0){
        list<Spectrum> in = spectra;
        list<Spectrum>::iterator iter;
        for(iter = in.begin();iter != in.end();iter++){
            removeSpectra(Spectrum(*iter));
        }
    }
}

void SpectralCluster::removeSpectra(const Spectrum &removed) {
    list<Spectrum>::iterator remove;
    for(remove=clusteredSpectra.begin();remove != clusteredSpectra.end();remove++){
        if(remove->getPeaks() == removed.getPeaks() && remove->getPrecursorMz() == removed.getPrecursorMz()){
            spectraIds.erase(remove->getId());
            clusteredSpectra.erase(remove);
        }
    }

}

bool SpectralCluster::isRemovedSupported() {
    return true;
}

string SpectralCluster::getProperty(string key) {
    return properties.getProperty(key);
}

void SpectralCluster::setProperty(string key, string value) {
    if(key == "") return;
    if(value == ""){
        properties.remove(key);
        return;
    }
    properties.setProperty(key,value);
}

Properties SpectralCluster::getProperties(){
    return properties;
}

bool SpectralCluster::storesPeakLists() {
    return true;

}

SpectralCluster::~SpectralCluster() {
}
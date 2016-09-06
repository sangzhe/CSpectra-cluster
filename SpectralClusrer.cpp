//
// Created by SangZhe on 16/8/31.
//

#include "SpectralClusrer.h"

Properties *properties = new Properties();

SpectralCluster::SpectralCluster(ICluster &copied, IConsensusSpectrumBuilder &consensusSpectrumBuilder) {
    new (this)SpectralCluster(copied.getId(),consensusSpectrumBuilder);
    if(!copied.storesPeakLists()) throw("Clusters can only be copied if peak lists are stored.");

    list<ISpectrum*> clusterdSpectra1 = copied.getClusterdSpectra();
    addSpectra(clusterdSpectra1);
}

SpectralCluster::SpectralCluster(string id, IConsensusSpectrumBuilder& consensusSpectrumBuilder) {
    this->id = id;
    *(this->consensusSpectrumBuilder) = consensusSpectrumBuilder;
}

set<string> SpectralCluster::getSpectralIds() {
    if(this->spectraIds.empty()){
       list<ISpectrum*> clusteredSpectra1 = getClusterdSpectra();
        list<ISpectrum*>::iterator iter;
        for(iter = clusteredSpectra1.begin();iter != clusteredSpectra1.end();iter++){
            spectraIds.insert((*iter)->getId());
        }
        const set<string> ret = spectraIds;
        return ret;
    }
}

string SpectralCluster::getSpectralId() {
    string sb;
    const set<string> spectraIds = getSpectralIds();
    if(spectraIds.size() > 1){
        set<string>::iterator iter = spectraIds.begin();
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
    ISpectrum* consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == nullptr) return 0;
    return consensusSpecrum1->getPrecursorMz();
}

int SpectralCluster::getPrecursorCharge() {
    ISpectrum* consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == nullptr) return 0;
    return consensusSpecrum1->getPrecursorCharge();
}



ISpectrum* SpectralCluster::getConsensusSpectrum() {
    if(clusteredSpectra.size() ==1) return *clusteredSpectra.begin();
    return consensusSpectrumBuilder->getConsensusSpectrum();
}

list<ISpectrum*> SpectralCluster::getClusterdSpectra() {
    return clusteredSpectra;
}

IConsensusSpectrumBuilder* SpectralCluster::getConsensusSpecrtrumBuilder() {
    return consensusSpectrumBuilder;
}

int SpectralCluster::getClusteredSpectraCount() {
    return clusteredSpectra.size();
}

void SpectralCluster::addSpectra(ISpectrum &merged) {
    ISpectrum *added = &merged;
    spectraIds.insert(merged.getId());
    if (*find(clusteredSpectra.begin(), clusteredSpectra.end(), added)) {
        clusteredSpectra.push_back(added);
    }
}

void SpectralCluster::addSpectra(list<ISpectrum *> &spectra) {
    if( spectra.size() > 0){
        bool spectrumAdded = false;
        list<ISpectrum*>::iterator iter;
        for(iter = spectra.begin();iter != spectra.end();iter++){
            addSpectra(*(*iter));
            }
    }
}

void SpectralCluster::removeSpectra(list<ISpectrum *> &spectra) {
    if (!isRemovedSupported())
        throw ("Remove not supported");
    if (spectra.size() > 0){
        list<ISpectrum*>::iterator iter;
        for(iter = spectra.begin();iter != spectra.end();iter++){

        }
    }
}

void SpectralCluster::removeSpectra(ISpectrum &removed) {
    list<ISpectrum*>::iterator remove;
    for(remove=clusteredSpectra.begin();remove != clusteredSpectra.end();remove++){
        if((*remove)->getPeaks() == removed.getPeaks() && (*remove)->getPrecursorMz() == removed.getPrecursorMz()){
            spectraIds.erase((*remove)->getId());
            clusteredSpectra.erase(remove);
        }
    }

}

bool SpectralCluster::isRemovedSupported() {
    return true;
}

string SpectralCluster::getProperty(string key) {
    return properties->getProperty(key);
}

void SpectralCluster::setProperty(string key, string value) {
    if(key == "") return;
    if(value == ""){
        properties->remove(key);
        return;
    }
    properties->setProperty(key,value);
}

Properties* SpectralCluster::getProperties(){
    return properties;
}

bool SpectralCluster::storesPeakLists() {
    return true;

}

SpectralCluster::~SpectralCluster() {
    delete properties,consensusSpectrumBuilder;
}
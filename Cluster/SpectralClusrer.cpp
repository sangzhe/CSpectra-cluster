//
// Created by SangZhe on 16/8/31.
//

#include "SpectralClusrer.h"

string SpectralCluster::MethodName = "SpectralCluster";


SpectralCluster::SpectralCluster(ICluster *copied, IConsensusSpectrumBuilder *consensusSpectrumBuilder) {
    new (this)SpectralCluster(copied->getId(),consensusSpectrumBuilder);
    if(!copied->storesPeakLists()) throw("Clusters can only be copied if peak lists are stored.");

    vector<ISpectrum*> clusterdSpectra1 = copied->getClusteredSpectra();
    addSpectra(clusterdSpectra1);
}

SpectralCluster::SpectralCluster(string id, IConsensusSpectrumBuilder* consensusSpectrumBuilder) {
    this->id = id;
    this->consensusSpectrumBuilder = consensusSpectrumBuilder;
    addSpectrumHolderListener(this->consensusSpectrumBuilder);
}

list<string> SpectralCluster::getSpectralIds() {
    if(this->spectralIds.empty()){
       for(ISpectrum* spectrum:clusteredSpectra){
           spectralIds.push_back(spectrum->getId());
       }
    }
    return spectralIds;

}

string SpectralCluster::getMethodName() {
    return MethodName;
}

string SpectralCluster::getSpectralId() {
    string sb;
    list<string> spectralIds = getSpectralIds();
    if(!spectralIds.empty()){
        list<string>::iterator iter = spectralIds.begin();
        string x = *iter;
        sb.append(*iter);
        ++iter;
        for(iter;iter != spectralIds.end();++iter){
            sb.append(",");
            sb.append(*iter);
        }
        return sb;
    }
    else {
        return *spectralIds.begin();
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
    ISpectrum *consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == nullptr) return 0;
    return consensusSpecrum1->getPrecursorMz();
}

int SpectralCluster::getPrecursorCharge() {
    ISpectrum *consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == nullptr) return 0;
    return consensusSpecrum1->getPrecursorCharge();
}



ISpectrum* SpectralCluster::getConsensusSpectrum() {
    if(clusteredSpectra.size() ==1) return (*clusteredSpectra.begin());
    return consensusSpectrumBuilder->getConsensusSpectrum();
}

vector<ISpectrum*> SpectralCluster::getClusteredSpectra() {
    return clusteredSpectra;
}

IConsensusSpectrumBuilder* SpectralCluster::getConsensusSpectrumBuilder() {
    return consensusSpectrumBuilder;
}

int SpectralCluster::getClusteredSpectraCount() {
    return clusteredSpectra.size();
}

void SpectralCluster::addSpectra( ISpectrum *merged) {
    vector<ISpectrum*>::iterator iter(find(clusteredSpectra.begin(),clusteredSpectra.end(),merged));
    if(iter == clusteredSpectra.end()){
        clusteredSpectra.push_back(merged);
        pointer_pool->add(merged);
        string Id = merged->getId();
        spectralIds.push_back(Id);
        vector<ISpectrum*> added(0);
        added.push_back(merged);
        notifySpectrumHolderListeners(true,added);

    }
//    if (*find(clusteredSpectra.begin(), clusteredSpectra.end(), added)) {
//        clusteredSpectra.push_back(added);
//    }
}

void SpectralCluster::addSpectra( vector<ISpectrum*>& spectra) {
    if( spectra.size() > 0){
        bool spectrumAdded = false;

        vector<ISpectrum*>::iterator iter;
        vector<ISpectrum*> ToAdd;
        for(iter = spectra.begin();iter != spectra.end();++iter){
            ISpectrum *added = *iter;
            string Id = added->getId();
            spectralIds.push_back(Id);
            vector<ISpectrum*> all = clusteredSpectra;
            vector<ISpectrum*>::iterator iter_1(find(all.begin(),all.end(),added));
            if(iter_1 == all.end()){
                spectrumAdded = true;
                clusteredSpectra.push_back(added);
                pointer_pool->add(added);

                ToAdd.push_back(added);
            }
        if(spectrumAdded){
            notifySpectrumHolderListeners(true,ToAdd);
        }
    }
    }
}

void SpectralCluster::removeSpectra( vector<ISpectrum*> &spectra) {
    if (!isRemovedSupported())
        throw ("Remove not supported");
    if (spectra.size() > 0){
        vector<ISpectrum*>::iterator iter;
        for(iter = spectra.begin();iter != spectra.end();++iter){
            removeSpectra(*iter);
        }
    }
}

void SpectralCluster::removeSpectra(ISpectrum *removed) {
    vector<ISpectrum*>::iterator remove;
    for(remove=clusteredSpectra.begin();remove != clusteredSpectra.end();++remove){
        vector<Peak> removePeaks = (*remove)->getPeaks();
        vector<Peak> removedPeaks = removed->getPeaks();
        if(removePeaks == removedPeaks && (*remove)->getPrecursorMz() == removed->getPrecursorMz()){
            list<string>::iterator iterator1 = find(spectralIds.begin(),spectralIds.end(),(*remove)->getId());
            if(iterator1 != spectralIds.end()){
                spectralIds.erase(iterator1);
            }
            pointer_pool->remove(*remove);
            clusteredSpectra.erase(remove);
        }
    }

}

void SpectralCluster::addSpectrumHolderListener(SpectrumHolderListener *added) {
    list<SpectrumHolderListener*>::iterator iter(find(spectrumHolderListeners.begin(),spectrumHolderListeners.end(),added));
    if(iter == spectrumHolderListeners.end()){
        spectrumHolderListeners.push_back(added);
        pointer_pool->add(added);
    }
}

void SpectralCluster::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(spectrumHolderListeners.begin(),spectrumHolderListeners.end(),removed));
    if(iter != spectrumHolderListeners.end()){
        spectrumHolderListeners.push_back(removed);
        pointer_pool->remove(removed);
    }
}

void SpectralCluster::notifySpectrumHolderListeners(bool isAdd, vector<ISpectrum *> spectra) {
    if (spectrumHolderListeners.empty())
        return;
    for (SpectrumHolderListener *listener:spectrumHolderListeners) {
        if (isAdd)
            listener->onSpectraAdd(this, spectra);
        else
            listener->onSpectraRemove(this, spectra);
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

list<ComparisonMatch> SpectralCluster::getComparisonMatches() {
    return list<ComparisonMatch>();
}

bool SpectralCluster::isKnownComparisonMatch(string clusterId) {
    return false;
}
string SpectralCluster::toString() {
    double precursorMZ = getPrecursorMz();
    string charge = "charge= ";
    string chargeValue = IOUtilities::IntToString(getPrecursorCharge(),"");
    string mz = ",mz= ";
    string mzValue = IOUtilities::FloatToString(precursorMZ,"10.3f");
    string count =  ",count= ";
    string countValue = IOUtilities::IntToString(clusteredSpectra.size(),"");
    string spectrum = ", spectrum = ";
    string text = charge + chargeValue + mz + mzValue + count + countValue + spectrum;
    for (ISpectrum *s : clusteredSpectra)
        text += s->getId() + ",";

    text = text.substr(0, text.length() - 1);
    return text;
}

SpectralCluster::~SpectralCluster() {
    pointer_pool->remove(consensusSpectrumBuilder);
    pointer_pool->remove(clusteredSpectra);
}
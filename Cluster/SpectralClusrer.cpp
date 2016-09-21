//
// Created by SangZhe on 16/8/31.
//

#include "SpectralClusrer.h"

string SpectralCluster::MethodName = "SpectralCluster";


SpectralCluster::SpectralCluster(ICluster *copied, IConsensusSpectrumBuilder *consensusSpectrumBuilder) {
    new (this)SpectralCluster(copied->getId(),consensusSpectrumBuilder);
    if(!copied->storesPeakLists()) throw("Clusters can only be copied if peak lists are stored.");

    list<ISpectrum*> clusterdSpectra1 = copied->getClusteredSpectra();
    addSpectra(clusterdSpectra1);
}

SpectralCluster::SpectralCluster(string id, IConsensusSpectrumBuilder* consensusSpectrumBuilder) {
    this->id = id;
    this->consensusSpectrumBuilder = consensusSpectrumBuilder;
}

unordered_set<string> SpectralCluster::getSpectralIds() {
    if(this->spectraIds.empty()){
       for(ISpectrum* spectrum:clusteredSpectra){
           spectraIds.insert(spectrum->getId());
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

list<ISpectrum*> SpectralCluster::getClusteredSpectra() const{
    return clusteredSpectra;
}

IConsensusSpectrumBuilder* SpectralCluster::getConsensusSpectrumBuilder() {
    return consensusSpectrumBuilder;
}

int SpectralCluster::getClusteredSpectraCount() {
    return clusteredSpectra.size();
}

void SpectralCluster::addSpectra(const ISpectrum *merged) {
    ISpectrum *added = new Spectrum(*merged);
    string Id = added->getId();
    spectraIds.insert(Id);
    list<ISpectrum*> all = clusteredSpectra;
    list<ISpectrum*>::iterator iter(find(all.begin(),all.end(),merged));
    if(iter == all.end()){
        clusteredSpectra.push_back(added);
        PointerPool::add(added);

        list<ISpectrum*> tmp;
        tmp.push_back(added);
        notifySpectrumHolderListeners(true,tmp);

    }
//    if (*find(clusteredSpectra.begin(), clusteredSpectra.end(), added)) {
//        clusteredSpectra.push_back(added);
//    }
}

void SpectralCluster::addSpectra(const list<ISpectrum*> &spectra) {
    if( spectra.size() > 0){
        bool spectrumAdded = false;

        list<ISpectrum*> in = spectra;
        list<ISpectrum*>::iterator iter;
        list<ISpectrum*> ToAdd;
        for(iter = in.begin();iter != in.end();iter++){
            ISpectrum *added = *iter;
            string Id = added->getId();
            spectraIds.insert(Id);
            list<ISpectrum*> all = clusteredSpectra;
            list<ISpectrum*>::iterator iter(find(all.begin(),all.end(),added));
            if(iter == all.end()){
                spectrumAdded = true;
                clusteredSpectra.push_back(added);
                PointerPool::add(added);

                ToAdd.push_back(added);
            }
        if(spectrumAdded){
            notifySpectrumHolderListeners(true,ToAdd);
        }
    }
    }
}

void SpectralCluster::removeSpectra(const list<ISpectrum*> &spectra) {
    if (!isRemovedSupported())
        throw ("Remove not supported");
    if (spectra.size() > 0){
        list<ISpectrum*> in = spectra;
        list<ISpectrum*>::iterator iter;
        for(iter = in.begin();iter != in.end();iter++){
            removeSpectra(*iter);
        }
    }
}

void SpectralCluster::removeSpectra(const ISpectrum *removed) {
    list<ISpectrum*>::iterator remove;
    for(remove=clusteredSpectra.begin();remove != clusteredSpectra.end();remove++){
        if((*remove)->getPeaks() == removed->getPeaks() && (*remove)->getPrecursorMz() == removed->getPrecursorMz()){
            unordered_set<string>::iterator iterator1 = find(spectraIds.begin(),spectraIds.end(),(*remove)->getId());
            if(iterator1 != spectraIds.end()){
                spectraIds.erase(iterator1);
            }
            clusteredSpectra.erase(remove);
            PointerPool::remove(*remove);
        }
    }

}

void SpectralCluster::addSpectrumHolderListener(SpectrumHolderListener *added) {
    list<SpectrumHolderListener*>::iterator iter(find(spectrumHolderListeners.begin(),spectrumHolderListeners.end(),added));
    if(iter == spectrumHolderListeners.end()){
        spectrumHolderListeners.push_back(added);
        PointerPool::add(added);
    }
}

void SpectralCluster::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(spectrumHolderListeners.begin(),spectrumHolderListeners.end(),removed));
    if(iter != spectrumHolderListeners.end()){
        spectrumHolderListeners.push_back(removed);
        PointerPool::add(removed);
    }
}

void SpectralCluster::notifySpectrumHolderListeners(bool isAdd, list<ISpectrum *> spectra) {
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
    PointerPool::remove(consensusSpectrumBuilder);
    PointerPool::remove(clusteredSpectra);
}
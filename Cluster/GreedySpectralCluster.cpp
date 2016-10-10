//
// Created by SangZhe on 16/9/9.
//

#include "GreedySpectralCluster.h"

string GreedySpectralCluster::MethodName = "GreedySpectralCluster";

int GreedySpectralCluster::SAVED_COMPARISON_MATCHES = 30;

GreedySpectralCluster::GreedySpectralCluster(string id) {
    this->id = id;
    this->consensusSpectrumBuilder = GreedyConsensusSpectrum::FACTORY->getGreedyConsensusSpectrumBuilder(id);
    addSpectrumHolderListener(this->consensusSpectrumBuilder);
}

GreedySpectralCluster::GreedySpectralCluster(ICluster* cluster) {
    this->id = cluster->getId();
    this->properties = cluster->getProperties();
    if(cluster->getMethodName() == MethodName){
        GreedySpectralCluster existingCluster = (GreedySpectralCluster)cluster;
        this->bestComparisonMatches.merge(existingCluster.bestComparisonMatches);
        this->lowestBestComparisonSimilarity = existingCluster.lowestBestComparisonSimilarity;
        this->bestComparisonMatchIds = existingCluster.bestComparisonMatchIds;
        // for greedy clusters the consensus spectrum must be copied since it cannot be derived from the actual spectra
        delete consensusSpectrumBuilder;
        consensusSpectrumBuilder = nullptr;
        this->consensusSpectrumBuilder =  (GreedyConsensusSpectrum*)existingCluster.getConsensusSpectrumBuilder();
        addSpectrumHolderListener(this->consensusSpectrumBuilder);

        pointer_pool->remove(clusteredSpectra);
        clusteredSpectra.clear();
        this->clusteredSpectra.insert(clusteredSpectra.end(),existingCluster.getClusteredSpectra().begin(),existingCluster.getClusteredSpectra().end()); // peak lists are already removed
        pointer_pool->add(clusteredSpectra);
        this->spectraIds.clear();

        for(ISpectrum *spectrum:clusteredSpectra){
            spectraIds.insert(spectrum->getId());
        }
    }else{
        delete consensusSpectrumBuilder;
        consensusSpectrumBuilder = nullptr;
        this->consensusSpectrumBuilder = GreedyConsensusSpectrum::FACTORY->getGreedyConsensusSpectrumBuilder(id);
        addSpectrumHolderListener(this->consensusSpectrumBuilder);


        if (!cluster->storesPeakLists())
            throw ("Cannot copy cluster without peak lists that is not a GreedyCluster.");
        addSpectra(clusteredSpectra);
    }

}

GreedySpectralCluster::GreedySpectralCluster(string id, vector<ISpectrum*> clusteredSpectra,
                                             GreedyConsensusSpectrum* consensusSpectrumBuilder,
                                             list<ComparisonMatch> &bestComparisonMatches) {
    this->id = id;
    pointer_pool->remove(clusteredSpectra);
    clusteredSpectra.clear();
    this->clusteredSpectra = clusteredSpectra;
    pointer_pool->add(clusteredSpectra);
    this->consensusSpectrumBuilder = consensusSpectrumBuilder;

    addSpectrumHolderListener(this->consensusSpectrumBuilder);
    setComparisonMatches(bestComparisonMatches);

    getSpectralIds();
}

unordered_set<string> GreedySpectralCluster::getSpectralIds(){
    if(spectraIds.empty()){
        for(ISpectrum *spectrum:clusteredSpectra){
            spectraIds.insert(spectrum->getId());
        }
    }
    return spectraIds;
}

string GreedySpectralCluster::getMethodName() {
    return MethodName;
}
string GreedySpectralCluster::getId() {
    if(id == ""){
        id = this->getUUID();
    }
    return id;
}

string GreedySpectralCluster::getSpectralId() {
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

void GreedySpectralCluster::setId(string id) {
    this->id = id;
}

float GreedySpectralCluster::getPrecursorMz() {
    ISpectrum *consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == nullptr) return 0;
    return consensusSpecrum1->getPrecursorMz();
}

int GreedySpectralCluster::getPrecursorCharge() {
    ISpectrum *consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == nullptr) return 0;
    return consensusSpecrum1->getPrecursorCharge();
}

vector<ISpectrum*> GreedySpectralCluster::getHighestQualitySpectra() {
    throw("unsupported");
}

ISpectrum* GreedySpectralCluster::getHighestQualitySpectrum() {
    return getConsensusSpectrum();
}

vector<ISpectrum*> GreedySpectralCluster::getClusteredSpectra() const {
    return vector<ISpectrum*>(clusteredSpectra);
}


IConsensusSpectrumBuilder* GreedySpectralCluster::getConsensusSpectrumBuilder() {
    return consensusSpectrumBuilder;
}

ISpectrum* GreedySpectralCluster::getConsensusSpectrum() {
    return consensusSpectrumBuilder->getConsensusSpectrum();
}

int GreedySpectralCluster::getClusteredSpectraCount() {
    return clusteredSpectra.size();
}

void GreedySpectralCluster::addSpectra(ISpectrum* merged) {
    unordered_set<string> Ids = spectraIds;
    unordered_set<string>::iterator iter(find(Ids.begin(),Ids.end(),merged->getId()));
    if( iter != Ids.end()) return;
    spectraIds.insert(merged->getId());
    ISpectrum *withoutPeaks = new Spectrum(*merged,vector<Peak>());
    pointer_pool->add(withoutPeaks);
    clusteredSpectra.push_back(withoutPeaks);
    vector<ISpectrum*> tmp = vector<ISpectrum*>();
    tmp.push_back(merged);
    notifySpectrumHolderListeners(true,tmp);

}

void GreedySpectralCluster::addSpectra(const vector<ISpectrum*> &spectra) {
    vector<ISpectrum*> Spectra = spectra;
    vector<ISpectrum*>::iterator iter;
    vector<ISpectrum*> ToAdd;
    bool spectrumAdded = false;
    for(iter = Spectra.begin();iter != Spectra.end();iter++){
        ISpectrum* added = *iter;
        unordered_set<string> Ids = spectraIds;
        unordered_set<string>::iterator iter(find(Ids.begin(),Ids.end(),added->getId()));
        if( iter != Ids.end()) continue;
        spectraIds.insert(added->getId());
        ISpectrum *withoutPeaks = new Spectrum(*added,vector<Peak>());
        pointer_pool->add(withoutPeaks);
        clusteredSpectra.push_back(withoutPeaks);
        spectrumAdded = true;

        ToAdd.push_back(added);
    }
    if(spectrumAdded){
        notifySpectrumHolderListeners(true,ToAdd);
    }
}

void GreedySpectralCluster::addSpectrumHolderListener(SpectrumHolderListener *added) {
    list<SpectrumHolderListener*>::iterator iter(find(spectrumHolderListeners.begin(),spectrumHolderListeners.end(),added));
    if(iter == spectrumHolderListeners.end()) {
        spectrumHolderListeners.push_back(added);
        pointer_pool->add(added);
    }
}

void GreedySpectralCluster::removeSpectrumHolderListener(SpectrumHolderListener *removed) {
    list<SpectrumHolderListener*>::iterator iter(find(spectrumHolderListeners.begin(),spectrumHolderListeners.end(),removed));
    if(iter != spectrumHolderListeners.end()) {
        spectrumHolderListeners.erase(iter);
        pointer_pool->remove(removed);
    }
}

void GreedySpectralCluster::notifySpectrumHolderListeners(bool isAdd, vector<ISpectrum *> spectra) {
    if (spectrumHolderListeners.empty())
        return;
    for (SpectrumHolderListener *listener : spectrumHolderListeners) {
        if (isAdd)
            listener->onSpectraAdd(this, spectra);
        else
            listener->onSpectraRemove(this, spectra);
    }
}

void GreedySpectralCluster::addCluster( ICluster *cluster) {
    if (cluster->storesPeakLists()) {
        addSpectra(cluster->getClusteredSpectra());
    }else{
        if (cluster->getClusteredSpectraCount() == 0 || cluster->getClusteredSpectra().empty())
            return;
        // simply add the consensus spectrum to this one
        consensusSpectrumBuilder->addConsensusSpectrum(*(cluster->getConsensusSpectrumBuilder()));

        // add the spectra and their ids
        clusteredSpectra.insert(clusteredSpectra.end(),cluster->getClusteredSpectra().begin(),cluster->getClusteredSpectra().end());

        pointer_pool->add(cluster->getClusteredSpectra());

        // save the spectra ids
        for (ISpectrum *spectrum : cluster->getClusteredSpectra())
            spectraIds.insert(spectrum->getId());

        notifySpectrumHolderListeners(true, cluster->getClusteredSpectra());   // tell other interested parties  true says this is an add
    }
}

bool GreedySpectralCluster::isRemovedSupported() {
    return false;
}

void GreedySpectralCluster::removeSpectra(const vector<ISpectrum *> &spectra) {
    throw("remove not support");

}

void GreedySpectralCluster::removeSpectra(ISpectrum *removed) {
    throw("remove not support");
}

string GreedySpectralCluster::getProperty(string key) {
    return properties.getProperty(key);
}

void GreedySpectralCluster::setProperty(string key, string value) {
    if (key == "")
        return;
    if (value == "") {
        properties.remove(key);
        return;
    }

    properties.setProperty(key, value);
}

Properties GreedySpectralCluster::getProperties() {
    return properties;
}

string GreedySpectralCluster::toString() {
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

void GreedySpectralCluster::saveComparisonResult(string id, float similarity) {
    if (bestComparisonMatches.size() >= SAVED_COMPARISON_MATCHES && similarity < lowestBestComparisonSimilarity)
        return;
    ComparisonMatch comparisonMatch(id, similarity);
    bestComparisonMatches.push_back(comparisonMatch);

    // remove the lowest matches if necessary
    if (bestComparisonMatches.size() >= SAVED_COMPARISON_MATCHES) {
        bestComparisonMatches.sort();
        int tooManyMatches = bestComparisonMatches.size() - SAVED_COMPARISON_MATCHES;
        for (int i = 0; i < tooManyMatches; i++)
            bestComparisonMatches.pop_front(); // natural order is lowest to highest, remove lowest

            lowestBestComparisonSimilarity = bestComparisonMatches.front().getSimilarity();
    }
    // if nothing is removed check whether the new lowest similarity is the lowest
    else if (similarity < lowestBestComparisonSimilarity) {
    lowestBestComparisonSimilarity = similarity;

    }

    bestComparisonMatchIds = unordered_set<string>(); // delete to mark as dirty
}

bool GreedySpectralCluster::isInBestComparisonResults(string id) {
    if (bestComparisonMatchIds == unordered_set<string>()) {
        bestComparisonMatchIds = unordered_set<string>(bestComparisonMatches.size());

        for (ComparisonMatch& comparisonMatch :bestComparisonMatches)
            bestComparisonMatchIds.insert(comparisonMatch.getSpectrumId());
    }
    unordered_set<string>::iterator iter = bestComparisonMatchIds.find(id);
    if(iter != bestComparisonMatchIds.end()){
        return true;
    }
    else return false;
}

bool GreedySpectralCluster::storesPeakLists() {
    return false;
}

list<ComparisonMatch> GreedySpectralCluster::getComparisonMatches() {
    return bestComparisonMatches;
}

void GreedySpectralCluster::setComparisonMatches(list<ComparisonMatch> comparisonMatches) {
    this->bestComparisonMatches.clear();
    if ( comparisonMatches.size() > 0) {
        this->bestComparisonMatches.merge(comparisonMatches);

        bestComparisonMatches.sort();
        lowestBestComparisonSimilarity = bestComparisonMatches.front().getSimilarity();
    } else {
        lowestBestComparisonSimilarity = 0;
    }

    bestComparisonMatchIds = unordered_set<string>(); // delete to mark as dirty
}

bool GreedySpectralCluster::isKnownComparisonMatch(string clusterId) {
    if (bestComparisonMatches.size() == 0)
        return false;

    for (ComparisonMatch& comparisonMatch : bestComparisonMatches) {
        if (comparisonMatch.getSpectrumId() == clusterId)
            return true;
    }

    return false;
}
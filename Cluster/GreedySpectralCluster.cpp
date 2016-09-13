//
// Created by SangZhe on 16/9/9.
//

#include "GreedySpectralCluster.h"

string GreedySpectralCluster::MethodName = "GreedySpectralCluster";

int GreedySpectralCluster::SAVED_COMPARISON_MATCHES = 30;

GreedySpectralCluster::GreedySpectralCluster(string id) {
    this->id = id;
    this->consensusSpectrumBuilder = GreedyConsensusSpectrum::FACTORY->getGreedyConsensusSpectrumBuilder(id);
}

GreedySpectralCluster::GreedySpectralCluster(ICluster& cluster) {
    this->id = cluster.getId();
    this->properties = cluster.getProperties();
    if(cluster.getMethodName() == MethodName){
        GreedySpectralCluster existingCluster = (GreedySpectralCluster)cluster;
        this->bestComparisonMatches.merge(existingCluster.bestComparisonMatches);
        this->lowestBestComparisonSimilarity = existingCluster.lowestBestComparisonSimilarity;
        this->bestComparisonMatchIds = existingCluster.bestComparisonMatchIds;
        // for greedy clusters the consensus spectrum must be copied since it cannot be derived from the actual spectra
        delete consensusSpectrumBuilder;
        this->consensusSpectrumBuilder =  (GreedyConsensusSpectrum*)existingCluster.getConsensusSpectrumBuilder();

        this->clusteredSpectra.merge(existingCluster.getClusteredSpectra()); // peak lists are already removed
        this->spectraIds.clear();

        list<Spectrum>::iterator iterator1;
        for(iterator1 = clusteredSpectra.begin();iterator1 != clusteredSpectra.end();iterator1++){
            spectraIds.insert(iterator1->getId());
        }
    }else{
        delete consensusSpectrumBuilder;
        this->consensusSpectrumBuilder = GreedyConsensusSpectrum::FACTORY->getGreedyConsensusSpectrumBuilder(id);

        if (!cluster.storesPeakLists())
            throw ("Cannot copy cluster without peak lists that is not a GreedyCluster.");
        addSpectra(clusteredSpectra);
    }

}

GreedySpectralCluster::GreedySpectralCluster(string id, list<Spectrum> clusteredSpectra,
                                             GreedyConsensusSpectrum* consensusSpectrumBuilder,
                                             list<ComparisonMatch> &bestComparisonMatches) {
    this->id = id;
    this->clusteredSpectra = clusteredSpectra;
    this->consensusSpectrumBuilder = consensusSpectrumBuilder;

//    addSpectrumHolderListener(this.consensusSpectrumBuilder);
    setComparisonMatches(bestComparisonMatches);

    getSpectralIds();
}

unordered_set<string> GreedySpectralCluster::getSpectralIds(){
    if(spectraIds.empty()){
        list<Spectrum> spectra = getClusteredSpectra();
        list<Spectrum>::iterator iterator1;
        for(iterator1 = spectra.begin();iterator1 != spectra.end();iterator1++){
            spectraIds.insert((*iterator1).getId());
        }
    }
    return spectraIds;
}

string GreedySpectralCluster::getId() {
    if(id == ""){
        char str[36];
        uuid_t uu;
        uuid_generate_random(uu);
        uuid_parse(str,uu);
        id = string(str);
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
    Spectrum consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == Spectrum()) return 0;
    return consensusSpecrum1.getPrecursorMz();
}

int GreedySpectralCluster::getPrecursorCharge() {
    Spectrum consensusSpecrum1 = getConsensusSpectrum();
    if(consensusSpecrum1 == Spectrum()) return 0;
    return consensusSpecrum1.getPrecursorCharge();
}

list<Spectrum> GreedySpectralCluster::getHighestQualitySpectra() {
    throw("unsupported");
}

Spectrum GreedySpectralCluster::getHighestQualitySpectrum() {
    return getConsensusSpectrum();
}

list<Spectrum> GreedySpectralCluster::getClusteredSpectra() const {
    return list<Spectrum>(clusteredSpectra);
}


IConsensusSpectrumBuilder* GreedySpectralCluster::getConsensusSpectrumBuilder() {
    return consensusSpectrumBuilder;
}

int GreedySpectralCluster::getClusteredSpectraCount() {
    return clusteredSpectra.size();
}

void GreedySpectralCluster::addSpectra(const Spectrum &merged) {

}
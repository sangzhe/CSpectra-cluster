//
// Created by SangZhe on 2016/9/27.
//

#include <iostream>
#include "GreedyIncrementalClusteringEngine.h"






GreedyIncrementalClusteringEngine::GreedyIncrementalClusteringEngine(ISimilarityChecker *sck, float windowSize,
                                                                     double clusteringPrecision,
                                                                     PeakFunction* spectrumFilterFunction,
                                                                     ClusterPredicate* clusterComparisonPredicate,
                                                                     int minNumberOfComparisons) {
    this->similarityChecker = sck;
    this->windowSize = windowSize;
    // this change is performed so that a high threshold means a high clustering quality
    this->mixtureProbability = 1 - clusteringPrecision;
    this->spectrumFilterFunction = spectrumFilterFunction;
    this->clusterComparisonPredicate = clusterComparisonPredicate;
    this->minNumberOfComparisons = minNumberOfComparisons;
    this->cumulativeDistributionFunction = CumulativeDistributionFunctionFactory::getDefaultCumlativeDistributionFunctionForSimilarityMetric(sck);

}

GreedyIncrementalClusteringEngine::GreedyIncrementalClusteringEngine(ISimilarityChecker* sck, float windowSize,
                                                                     double clusteringPrecision,
                                                                     PeakFunction* spectrumFilterFunction) {
    new(this)GreedyIncrementalClusteringEngine(sck, windowSize, clusteringPrecision, spectrumFilterFunction, nullptr);
}

GreedyIncrementalClusteringEngine::GreedyIncrementalClusteringEngine(ISimilarityChecker* sck, float windowSize,
                                                                     double clusteringPrecision,
                                                                     PeakFunction* spectrumFilterFunction,
                                                                     ClusterPredicate* clusterComparisonPredicate) {
    new(this)GreedyIncrementalClusteringEngine(sck, windowSize, clusteringPrecision, spectrumFilterFunction, clusterComparisonPredicate,
         Defaults::getMinNumberComparisons());
};

double GreedyIncrementalClusteringEngine::getWindowSize() {
    return windowSize;
}

int GreedyIncrementalClusteringEngine::getCurrentMZ() {
    return currentMZAsInt;
}

void GreedyIncrementalClusteringEngine::setCurrentMZ(const double &pCurrentMZ) {
    int test = MZIntensityUtilities::mzToInt(pCurrentMZ);
    const int currentMZ = getCurrentMZ();
    if (currentMZ > test) {  // all ow roundoff error but not much
        double del = currentMZ - test;  // difference

        if (fabs(del) > MZIntensityUtilities::MZ_RESOLUTION * MZIntensityUtilities::SMALL_MZ_DIFFERENCE) {
            throw("mz values MUST be added in order - was ");
//                                            + NumberUtilities.formatDouble(currentMZ, 3) + " new " +
//                                            NumberUtilities.formatDouble(pCurrentMZ, 3) + " del  " +
//                                            del
//            );

        }

    }
    currentMZAsInt = test;
}

vector<ICluster*> GreedyIncrementalClusteringEngine::getClusters() {
    vector<ICluster*> ret(clusters.begin(),clusters.end());
    sort(ret.begin(),ret.end(),ICluster::cmp);
    pointer_pool->add(ret);
    return ret;
}

void GreedyIncrementalClusteringEngine::addClusters(ICluster *cluster) {
    throw("use addClusterIncremental instead");
}
void GreedyIncrementalClusteringEngine::addClusters(vector<ICluster *> cluster) {
    throw("use addClusterIncremental instead");
}

vector<ICluster*> GreedyIncrementalClusteringEngine::addClusterIncremental(ICluster *added) {
    double precursorMz = added->getPrecursorMz();


    vector<ICluster*> clustersToremove = findClustersTooLow(precursorMz);

    // either add as an existing cluster if make a new cluster
    addToClusters(added);

    return clustersToremove;
}


vector<ICluster*> GreedyIncrementalClusteringEngine::findClustersTooLow(double precursorMz) {
    setCurrentMZ(precursorMz); // also performs sanity check whether precursorMz is larger than currentMz

    double windowSize1 = getWindowSize();
    double lowestMZ = precursorMz - windowSize1;
    vector<ICluster*> clustersToremove ;
    vector<ISpectrum*> consensusSpectraToRemove ;

    for (int i = 0; i < clusters.size(); i++) {
        ICluster* currentCluster = clusters[i];
        ISpectrum* currentConsensusSpec = filteredConsensusSpectra[i];

        float testPrecursorMz = currentCluster->getPrecursorMz();
        if (lowestMZ > testPrecursorMz) {
            clustersToremove.push_back(currentCluster);
            consensusSpectraToRemove.push_back(currentConsensusSpec);
        }
    }
    if (!clustersToremove.empty()) {
        for(int i=0;i<clustersToremove.size();i++){
            vector<GreedySpectralCluster*>::iterator removedCluster(find(clusters.begin(),clusters.end(),clustersToremove[i]));
            if(removedCluster != clusters.end()){
//                pointer_pool->remove(*removedCluster);
                clusters.erase(removedCluster);

            }
        }

        for(int i = 0;i<consensusSpectraToRemove.size();i++){
            vector<ISpectrum*>::iterator removedSpectrum(find(filteredConsensusSpectra.begin(),filteredConsensusSpectra.end(),consensusSpectraToRemove[i]));
            if(removedSpectrum != filteredConsensusSpectra.end()){
                delete (*removedSpectrum);
                filteredConsensusSpectra.erase(removedSpectrum);
            }
        }
    }

    return clustersToremove;
}

void GreedyIncrementalClusteringEngine::addToClusters(ICluster *clusterToAdd) {
    GreedySpectralCluster* greedySpectralCluster = convertToGreedyCluster(clusterToAdd);

    // if there are no clusters yet, just save it
    if (clusters.empty()) {
        clusters.push_back(greedySpectralCluster);
        ISpectrum* filteredSpectrum = filterSpectrum(greedySpectralCluster->getConsensusSpectrum());
        filteredConsensusSpectra.push_back(filteredSpectrum);
        return;
    }

    ISimilarityChecker* sCheck = getSimilarityChecker();
    // always only compare the N highest peaks
    ISpectrum* filteredConsensusSpectrumToAdd = filterSpectrum(clusterToAdd->getConsensusSpectrum());
//        System.out.println("Size of clusterdSpectrum: "+ clusters.size()+"\n");

    // add once an acceptable similarity score is found
    // this version does not look for the best match
    int nComparisons = clusters.size();

    if (nComparisons < minNumberOfComparisons)
        nComparisons = minNumberOfComparisons;


    for (int i = 0; i < clusters.size(); i++) {
        GreedySpectralCluster* existingCluster = clusters[i];

        // apply the predicate if needed
        if (clusterComparisonPredicate != nullptr) {
            if (!clusterComparisonPredicate->apply(clusterToAdd, existingCluster))
                continue;
        }

        ISpectrum* filteredConsensusSpectrum = filteredConsensusSpectra[i];

        double similarityScore = sCheck->assessSimilarity(filteredConsensusSpectrum, filteredConsensusSpectrumToAdd);


        if (cumulativeDistributionFunction.isSaveMatch(similarityScore, nComparisons, mixtureProbability)) {
            // use the originally passed cluster object for this, the greedy version is only used
            // to track comparison results and used if added internally

            // preserve the id of the larger cluster
            if (clusterToAdd->getClusteredSpectraCount() > existingCluster->getClusteredSpectraCount())
                existingCluster->setId(clusterToAdd->getId());

            // add to cluster
            existingCluster->addCluster(clusterToAdd);

            // update the existing consensus spectrum
            ISpectrum* removed  = filteredConsensusSpectra[i];
//            delete removed;
            filteredConsensusSpectra[i] =  filterSpectrum(existingCluster->getConsensusSpectrum());
//            pointer_pool->remove(removed);

            // since the cluster was added we're done
            return;
        }

        // save the comparison result for the next round of clustering
        greedySpectralCluster->saveComparisonResult(existingCluster->getId(), (float) similarityScore);
        existingCluster->saveComparisonResult(greedySpectralCluster->getId(), (float) similarityScore);
    }
//
//        long end = System.currentTimeMillis();
//
//        System.out.println("For loop containing similarity assessment time: "+ (double)(end - start)/1000+"\n");

    // since the cluster wasn't merged, add it as new
    clusters.push_back(greedySpectralCluster);

    // process the consensus spectrum
    ISpectrum* filteredConsensusSpectrum = filterSpectrum(greedySpectralCluster->getConsensusSpectrum());
    filteredConsensusSpectra.push_back(filteredConsensusSpectrum);
    pointer_pool->remove(clusterToAdd);
}

ISpectrum* GreedyIncrementalClusteringEngine::filterSpectrum(ISpectrum *spectrumToFilter) {
//    when a ConsensusSpectrum is added to filteredConsensusSpectra
//    the life cycle of filteredSpectrum start from
//    here-->push_back() to filteredConsensusSpectrum--> exist until the end of program / replaced with another filteredSpectrum--->delete
    if (spectrumFilterFunction == nullptr)
        return spectrumToFilter;
    ISpectrum* filteredSpectrum;
    string nHighestPeaks = spectrumToFilter->getProperty(KnownProperties::N_HIGHEST_PEAKS);
    if (nHighestPeaks != "") {
        int highestPeaks = IOUtilities::StringToInt(nHighestPeaks);
        filteredSpectrum = spectrumToFilter->getHighestNPeaks(highestPeaks);
    }
    else {
        vector<Peak> toFilter =spectrumToFilter->getPeaks();
        vector<Peak> filterdPeak = spectrumFilterFunction->apply(toFilter);
        filteredSpectrum = new Spectrum(*spectrumToFilter, filterdPeak);
        filteredSpectrum->setProperty(KnownProperties::N_HIGHEST_PEAKS, IOUtilities::IntToString(filteredSpectrum->getPeaks().size(),""));
    }

    return filteredSpectrum;
}

GreedySpectralCluster* GreedyIncrementalClusteringEngine::convertToGreedyCluster(ICluster *cluster) {
    // change the clusterToAdd to a 'GreedyCluster'
    GreedySpectralCluster *greedyCluster;
    if (cluster->getMethodName() == "GreedySpectralCluster") {
        greedyCluster = (GreedySpectralCluster*) cluster;
    }
    else {
        greedyCluster = new GreedySpectralCluster(cluster);
        pointer_pool->add(greedyCluster);
    }

    return greedyCluster;
}

bool GreedyIncrementalClusteringEngine::processClusters() {
    throw("not supported");
}

ISimilarityChecker* GreedyIncrementalClusteringEngine::getSimilarityChecker() {
    return similarityChecker;
}

double GreedyIncrementalClusteringEngine::getSimilarityThreshold() {
    return 1 - mixtureProbability;
}

int GreedyIncrementalClusteringEngine::size() {
    return clusters.size();
}

GreedyIncrementalClusteringEngine::~GreedyIncrementalClusteringEngine() {
    delete spectrumFilterFunction;
    delete clusterComparisonPredicate;
    delete similarityChecker;
    for(ICluster* cluster:clusters){
        pointer_pool->remove(cluster);
    }
    for(ISpectrum* spectrum:filteredConsensusSpectra){
        delete spectrum;
    }
}
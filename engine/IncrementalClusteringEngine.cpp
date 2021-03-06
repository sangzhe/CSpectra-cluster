//
// Created by SangZhe on 2016/9/26.
//

#include "IncrementalClusteringEngine.h"
#include "../util/MZIntensityUtilities.h"
#include "../Cluster/SpectralClusrer.h"
#include "../util/Defaults.h"

double IncrementalClusteringEngine::PROPORTION_SHARED_SPECTRA_FOR_IDENTICAL = 1;

int IncrementalClusteringEngine::numberNotMerge = 0;
int IncrementalClusteringEngine::numberGoodMerge = 0;
int IncrementalClusteringEngine::numberLessGoodMerge = 0;

IncrementalClusteringEngine::IncrementalClusteringEngine(ISimilarityChecker* sck, float windowSize,
                                                         double similarityThreshold) {
    this->similarityChecker = sck;
    this->windowSize = windowSize;
    this->similarityThreshold = similarityThreshold;
}

double IncrementalClusteringEngine::getWindowSize() {
    return windowSize;
}

int IncrementalClusteringEngine::getCurrentMZ()const {
    return currentMZAsInt;
}

void IncrementalClusteringEngine::setCurrentMZ(const double pCurrentMZ) {
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
//            ToDo fix throw;

        }

    }
    currentMZAsInt = test;
}

vector<ICluster*> IncrementalClusteringEngine::getClusters() {
    vector<ICluster*> ret(clusters);
    sort(ret.begin(),ret.end(),ICluster::cmp);
}

void IncrementalClusteringEngine::addClusters(ICluster *cluster) {

}
void IncrementalClusteringEngine::addClusters(const vector<ICluster *> &cluster) {
    throw("not supported");
}

vector<ICluster*> IncrementalClusteringEngine::addClusterIncremental(ICluster *added) {
    double precursorMz = added->getPrecursorMz();
    vector<ICluster*> clustersToremove = findClustersTooLow(precursorMz);
    // either add as an existing cluster if make a new cluster
    addToClusters(added);
    return clustersToremove;

}

vector<ICluster*> IncrementalClusteringEngine::findClustersTooLow(double precursorMz) {
    setCurrentMZ(precursorMz); // also performs sanity check whether precursorMz is larger than currentMz

    double windowSize1 = getWindowSize();
    double lowestMZ = precursorMz - windowSize1;
    vector<ICluster*> clustersToremove;
    vector<ICluster*> myClusters = internalGetClusters();
    for (ICluster *test : myClusters) {
        float testPrecursorMz = test->getPrecursorMz();
        if (lowestMZ > testPrecursorMz) {
            clustersToremove.push_back(test);
            pointer_pool->add(test);
        }
    }
    if (!clustersToremove.empty()){
        // might break hear
        for(ICluster* cluster:clustersToremove){
            vector<ICluster*>::iterator removedCluster(find(clusters.begin(),clusters.end(),cluster));
            if(removedCluster != clusters.end()){
                pointer_pool->remove(*removedCluster);
                clusters.erase(removedCluster);
            }
        }
    }
    return clustersToremove;
}

void IncrementalClusteringEngine::addToClusters(ICluster *clusterToAdd) {
    vector<ICluster*> myClusters = internalGetClusters();
    if (myClusters.empty()) {   // no checks just add
        ICluster *add = new SpectralCluster(clusterToAdd, Defaults::getDefaultConsensusSpectrumBuilder());
        myClusters.push_back(add);
        pointer_pool->add(add);
        pointer_pool->remove(clusterToAdd);
        numberNotMerge++;
        return;
    }

    //  handle the case of subsets and almost subsets
    if (handleFullContainment(clusterToAdd))
        return; // no need to add we are contained


    ISimilarityChecker *sCheck = getSimilarityChecker();
    vector<ISpectrum*> clusteredSpectra1 = clusterToAdd->getClusteredSpectra();

    double highestSimilarityScore = 0;
    ICluster* mostSimilarCluster = nullptr;

    ISpectrum* consensusSpectrum1 = clusterToAdd->getConsensusSpectrum();  // sub spectra are really only one spectrum clusters

//        System.out.println("Size of clusterdSpectrum: "+ myClusters.size()+"\n");

//        long start = System.currentTimeMillis();
    // find the cluster with the highest similarity score
    for (ICluster *cluster : myClusters) {
        ISpectrum *consensusSpectrum = cluster->getConsensusSpectrum();

        double similarityScore = sCheck->assessSimilarity(consensusSpectrum, consensusSpectrum1);

        if (similarityScore > highestSimilarityScore && similarityScore >= similarityThreshold) {
            highestSimilarityScore = similarityScore;
            mostSimilarCluster = cluster;
        }
    }
//
//        long end = System.currentTimeMillis();
//
//        System.out.println("For loop containing similarity assessment time: "+ (double)(end - start)/1000+"\n");

    if (mostSimilarCluster != nullptr) {
        // Preserve the cluster id from the bigger cluster, in terms of number of spectra
        // This is used to facilitate incremental clustering
        if (mostSimilarCluster->getClusteredSpectraCount() < clusterToAdd->getClusteredSpectraCount()) {
            mostSimilarCluster->setId(clusterToAdd->getId());
        }

        // add to cluster
        vector<ISpectrum*> clusteredSpectra(clusteredSpectra1.size());
        vector<ISpectrum*> merged = clusteredSpectra1;
        mostSimilarCluster->addSpectra(merged);

        numberGoodMerge++;
    } else {
        // create a new cluster
        ICluster* add = new SpectralCluster(clusterToAdd, Defaults::getDefaultConsensusSpectrumBuilder());
        pointer_pool->add(add);
        pointer_pool->remove(clusterToAdd);
        myClusters.push_back(add);
        numberNotMerge++;
    }
}

bool IncrementalClusteringEngine::handleFullContainment(ICluster *clusterToAdd) {
    vector<ICluster*> myclusters = internalGetClusters();
    ICluster *toReplace = nullptr;
    double maxProportionSharedSpectra = IOUtilities::FLOAT_MAX_VALUE;
    for (ICluster *myCluster : myclusters) {
        double proportionSharedSpectra = getProportionSharedSpectraIds(clusterToAdd, myCluster);

        if (proportionSharedSpectra > maxProportionSharedSpectra) {
            maxProportionSharedSpectra = proportionSharedSpectra;
            toReplace = myCluster;
            if (proportionSharedSpectra == 1)   // is full subset
                break;
        }
    }

    if (maxProportionSharedSpectra >= PROPORTION_SHARED_SPECTRA_FOR_IDENTICAL) {
        mergeIntoCluster(clusterToAdd, toReplace);
        return true; // done
    }
    return false;
}

list<string> IncrementalClusteringEngine::getSharedSpectraIds(const list<string> &firstIds,
                                                                        ICluster *c2) {

    list<string> list1(firstIds);
    list<string> list2 = c2->getSpectralIds();
    list<string> ret;
    for(string id1:list1){
        for(string id2: list2){
            if (id1 == id2)
                ret.push_back(id1);
        }
    }

    return ret;
}

double IncrementalClusteringEngine::getProportionSharedSpectraIds( ICluster *cluster1,  ICluster *cluster2) {
    int sharedSpectraIds = getSharedSpectraIds(cluster1->getSpectralIds(), cluster2).size();

    int minSize = min(cluster1->getClusteredSpectraCount(), cluster2->getClusteredSpectraCount());

    return (double) sharedSpectraIds / minSize;
}

void IncrementalClusteringEngine::mergeIntoCluster(ICluster *mergeFrom, ICluster *mergeInto) {
    vector<ISpectrum*> clusteredSpectra1 = mergeFrom->getClusteredSpectra();
    vector<ISpectrum*> clusteredSpectra(clusteredSpectra1.size());
    vector<ISpectrum*> merged = clusteredSpectra1;
    mergeInto->addSpectra(merged);
    numberLessGoodMerge++;
}

bool IncrementalClusteringEngine::processClusters() {
    throw("Don\'t do this using an IncrementalClusteringEngine use a WrappedIncrementalClusteringEngine"); // ToDo

}

vector<ICluster*> IncrementalClusteringEngine::internalGetClusters() {
    return clusters;
}

ISimilarityChecker* IncrementalClusteringEngine::getSimilarityChecker() {
    return similarityChecker;
}

double IncrementalClusteringEngine::getSimilarityThreshold() {
    return similarityThreshold;
}

int IncrementalClusteringEngine::size() {
    return clusters.size();
}

string IncrementalClusteringEngine::toString() {

}

IncrementalClusteringEngine::~IncrementalClusteringEngine() {
    pointer_pool->remove(clusters);
    delete similarityChecker;
}
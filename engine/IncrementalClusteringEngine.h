//
// Created by SangZhe on 2016/9/26.
//

#ifndef CSPECTRA_CLUSTER_INCREMENTALCLUSTERINGENGINE_H
#define CSPECTRA_CLUSTER_INCREMENTALCLUSTERINGENGINE_H


#include "IIncrementalClusteringEngine.h"
#include "../pool/PoolFactory.h"

class IncrementalClusteringEngine:public IIncrementalClusteringEngine {
public:
    static  double PROPORTION_SHARED_SPECTRA_FOR_IDENTICAL;

    static int numberNotMerge ;
    static int numberGoodMerge ;
    static int numberLessGoodMerge ;

    IncrementalClusteringEngine(ISimilarityChecker* sck, float windowSize,double similarityThreshold);

    double getWindowSize();

    int getCurrentMZ() const;

    void setCurrentMZ(const double pCurrentMZ);

    ISimilarityChecker* getSimilarityChecker();

    double getSimilarityThreshold();

    vector<ICluster*> getClusters();

    void addClusters(const vector<ICluster*>& cluster);

    void addClusters(ICluster* cluster);

    bool processClusters();

    int size();

    vector<ICluster*> addClusterIncremental(ICluster* added);

    static unordered_set<string> getSharedSpectraIds(const unordered_set<string>& firstIds,  ICluster* c2);

    static double getProportionSharedSpectraIds( ICluster* cluster1,   ICluster* cluster2);

    string toString();

    ~IncrementalClusteringEngine();


protected:
    vector<ICluster*> findClustersTooLow(double precursorMz);
    void addToClusters( ICluster* clusterToAdd);
    bool handleFullContainment( ICluster* clusterToAdd);

    void mergeIntoCluster( ICluster* mergeFrom,  ICluster* mergeInto);
    vector<ICluster*> internalGetClusters();

private:
    PointerPool* pointer_pool = PoolFactory::getInstance();
    vector<ICluster*> clusters;
    ISimilarityChecker* similarityChecker;
    double windowSize;
    double similarityThreshold;
    int currentMZAsInt;
};


#endif //CSPECTRA_CLUSTER_INCREMENTALCLUSTERINGENGINE_H

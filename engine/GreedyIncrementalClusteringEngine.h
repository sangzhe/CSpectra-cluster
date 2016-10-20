//
// Created by SangZhe on 2016/9/27.
//

#ifndef CSPECTRA_CLUSTER_GREEDYINCREMENTALCLUSTERINGENGINE_H
#define CSPECTRA_CLUSTER_GREEDYINCREMENTALCLUSTERINGENGINE_H


#include "IIncrementalClusteringEngine.h"
#include "../Cluster/GreedySpectralCluster.h"
#include "../cdf/CumulativeDistributionFunction.h"
#include "../cdf/CumulativeDistributionFunctionFactory.h"
#include "../Spectrum/KnownProperties.h"
#include "../util/Predicate/cluster_comparison/ClusterPredicate.h"

class GreedyIncrementalClusteringEngine: public IIncrementalClusteringEngine {
private:
    PointerPool* pointer_pool = PoolFactory::getInstance();

    vector<GreedySpectralCluster*> clusters;
    vector<ISpectrum*> filteredConsensusSpectra;

    ISimilarityChecker *similarityChecker;
    double windowSize;
    double mixtureProbability;
    CumulativeDistributionFunction cumulativeDistributionFunction;
    PeakFunction* spectrumFilterFunction;
    ClusterPredicate* clusterComparisonPredicate;

    int currentMZAsInt =0 ;
    int minNumberOfComparisons;

    ISpectrum* filterSpectrum(ISpectrum* spectrumToFilter);

    GreedySpectralCluster* convertToGreedyCluster(ICluster* cluster);

public:

    GreedyIncrementalClusteringEngine(ISimilarityChecker *sck, float windowSize, double clusteringPrecision,
                                      PeakFunction* spectrumFilterFunction,
                                      ClusterPredicate* clusterComparisonPredicate,
                                      int minNumberOfComparisons);

    GreedyIncrementalClusteringEngine(ISimilarityChecker* sck,
                                      float windowSize,
                                      double clusteringPrecision,
                                      PeakFunction* spectrumFilterFunction,
                                      ClusterPredicate* clusterComparisonPredicate);

    GreedyIncrementalClusteringEngine(ISimilarityChecker* sck,
                                      float windowSize,
                                      double clusteringPrecision,
                                      PeakFunction* spectrumFilterFunction);

    double getWindowSize();

    int getCurrentMZ();

    void setCurrentMZ(const double &pCurrentMZ);

    ISimilarityChecker* getSimilarityChecker();

    double getSimilarityThreshold();

    vector<ICluster*> getClusters();

    void addClusters(vector<ICluster*> cluster);

    void addClusters(ICluster* cluster);

    vector<ICluster*> addClusterIncremental(ICluster* added);


    bool processClusters();

    int size();

    ~GreedyIncrementalClusteringEngine();

protected:
    vector<ICluster*> findClustersTooLow(double precursorMz);

    void addToClusters(ICluster* clusterToAdd);


};


#endif //CSPECTRA_CLUSTER_GREEDYINCREMENTALCLUSTERINGENGINE_H

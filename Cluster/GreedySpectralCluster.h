//
// Created by SangZhe on 16/9/9.
//

#ifndef CSPECTRA_CLUSTER_GREEDYSPECTRALCLUSTER_H
#define CSPECTRA_CLUSTER_GREEDYSPECTRALCLUSTER_H


#include "ICluster.h"
#include "../Consensus/GreedyConsensusSpectrum.h"

class GreedySpectralCluster: public ICluster {
public:
    static  int SAVED_COMPARISON_MATCHES;

    GreedySpectralCluster(string id);
    GreedySpectralCluster(ICluster* cluster);
    GreedySpectralCluster(string id, vector<ISpectrum*> clusteredSpectra, GreedyConsensusSpectrum *consensusSpectrumBuilder, list<ComparisonMatch> &bestComparisonMatches);

    string getMethodName();
    unordered_set<string> getSpectralIds();

    string getId();

    string getSpectralId();

    void setId(string id);

    float getPrecursorMz();

    int getPrecursorCharge();

    vector<ISpectrum*> getHighestQualitySpectra();

    ISpectrum* getHighestQualitySpectrum();

    ISpectrum* getConsensusSpectrum();

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();

    vector<ISpectrum*> getClusteredSpectra() const;

    int getClusteredSpectraCount();

    void addCluster(ICluster* cluster);

    void addSpectra(const ISpectrum* merged);
    void addSpectra(const vector<ISpectrum*>& spectra);


    bool isRemovedSupported();
    void removeSpectra(const ISpectrum* removed);
    void removeSpectra(const vector<ISpectrum*> &spectra);

    string getProperty(string key);

    void setProperty(string key, string value);

    Properties getProperties();

    string toString();

    void saveComparisonResult(string id, float similarity);

    bool isInBestComparisonResults(string id);

    bool storesPeakLists();

    list<ComparisonMatch> getComparisonMatches();

    void setComparisonMatches(list<ComparisonMatch> comparisonMatches);

    bool isKnownComparisonMatch(string clusterId);

    void addSpectrumHolderListener(SpectrumHolderListener* added) ;
    void removeSpectrumHolderListener(SpectrumHolderListener* removed);


//    bool operator ==(ICluster& O);

private:
    PointerPool* pointer_pool = PoolFactory::getInstance();
    static string MethodName;
    list<ComparisonMatch> bestComparisonMatches;
    float lowestBestComparisonSimilarity = 0;
    unordered_set<string> bestComparisonMatchIds;
    string id;
    unordered_set<string> spectraIds;
    Properties properties ;

    list<SpectrumHolderListener*> spectrumHolderListeners;

    vector<ISpectrum*> clusteredSpectra;
    GreedyConsensusSpectrum* consensusSpectrumBuilder;
    void notifySpectrumHolderListeners(bool isAdd, vector<ISpectrum*> spectra);


};


#endif //CSPECTRA_CLUSTER_GREEDYSPECTRALCLUSTER_H

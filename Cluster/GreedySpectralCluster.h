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
    GreedySpectralCluster(ICluster& cluster);
    GreedySpectralCluster(string id, list<Spectrum> clusteredSpectra, GreedyConsensusSpectrum *consensusSpectrumBuilder, list<ComparisonMatch> &bestComparisonMatches);

    string getMethodName();
    unordered_set<string> getSpectralIds();

    string getId();

    string getSpectralId();

    void setId(string id);

    float getPrecursorMz();

    int getPrecursorCharge();

    list<Spectrum> getHighestQualitySpectra();

    Spectrum getHighestQualitySpectrum();

    Spectrum getConsensusSpectrum();

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();

    list<Spectrum> getClusteredSpectra() const;

    int getClusteredSpectraCount();

    void addSpectra(const Spectrum& merged);
    void addSpectra(const list<Spectrum>& spectra);
    bool isRemovedSupported();
    void removeSpectra(const Spectrum& removed);
    void removeSpectra(const list<Spectrum> &spectra);

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

//    bool operator ==(ICluster& O);

private:
    static string MethodName;
    list<ComparisonMatch> bestComparisonMatches;
    float lowestBestComparisonSimilarity = 0;
    unordered_set<string> bestComparisonMatchIds;
    string id;
    unordered_set<string> spectraIds;
    Properties properties ;
    /* ToDo HolderListener to inform update status*/
    //List<SpectrumHolderListener> spectrumHolderListeners = new CopyOnWriteArrayList<SpectrumHolderListener>();
    list<Spectrum> clusteredSpectra;
    GreedyConsensusSpectrum* consensusSpectrumBuilder;


};


#endif //CSPECTRA_CLUSTER_GREEDYSPECTRALCLUSTER_H

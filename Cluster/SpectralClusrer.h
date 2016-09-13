//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_SPECTRALCLUSRER_H
#define CSPECTRA_CLUSTER_SPECTRALCLUSRER_H


#include "ICluster.h"
#include "../Property/Properties.h"
#include "algorithm"

class SpectralCluster: public ICluster{
private:
    static string MethodName;

    string id;
    list<Spectrum> clusteredSpectra;
    Properties properties;
    unordered_set<string> spectraIds;
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;

public:
    SpectralCluster( ICluster& copied,IConsensusSpectrumBuilder& consensusSpectrumBuilder);

    SpectralCluster(string id, IConsensusSpectrumBuilder& consensusSpectrumBuilder);

    unordered_set<string> getSpectralIds();

    string getMethodName();

    string getSpectralId();

    string getId();

    float getPrecursorMz();

    int getPrecursorCharge();

    Spectrum getConsensusSpectrum();

    list<Spectrum> getClusteredSpectra();

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();


    int getClusteredSpectraCount();

    string getProperty(string key);

    Properties getProperties();

    void setId(string id);

    void setProperty(string key,string value);

    void addSpectra(const Spectrum& merged);

    void addSpectra(const list<Spectrum>& spectra);

    void removeSpectra(const Spectrum& removed);

    void removeSpectra(const list<Spectrum> &spectra) ;

    bool isRemovedSupported();

    bool storesPeakLists();

    ~SpectralCluster();



};


#endif //CSPECTRA_CLUSTER_SPECTRALCLUSRER_H

//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_SPECTRALCLUSRER_H
#define CSPECTRA_CLUSTER_SPECTRALCLUSRER_H


#include "ICluster.h"
#include "Properties.h"

class SpectralCluster: public ICluster{
private:
    string id;
    list<ISpectrum*> clusteredSpectra;
    Properties* properties;
    set<string> spectraIds;
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;

public:
    SpectralCluster( ICluster& copied,IConsensusSpectrumBuilder& consensusSpectrumBuilder);

    SpectralCluster(string id, IConsensusSpectrumBuilder& consensusSpectrumBuilder);

    set<string> getSpectralIds();

    string getSpectralId();

    string getId();

    float getPrecursorMz();

    int getPrecursorCharge();

    ISpectrum* getConsensusSpectrum();

    list<ISpectrum*> getClusterdSpectra();

    IConsensusSpectrumBuilder* getConsensusSpecrtrumBuilder();


    int getClusteredSpectraCount();

    string getProperty(string key);

    Properties* getProperties();

    void setId(string id);

    void setProperty(string key,string value);

    void addSpectra(ISpectrum& merged);

    void addSpectra(list<ISpectrum*>& spectra);

    void removeSpectra( ISpectrum& removed);

    void removeSpectra( list<ISpectrum*> &spectra) ;

    bool isRemovedSupported();

    bool storesPeakLists();

    ~SpectralCluster();



};


#endif //CSPECTRA_CLUSTER_SPECTRALCLUSRER_H

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

    list<SpectrumHolderListener*> spectrumHolderListeners;

    list<ISpectrum*> clusteredSpectra;

    Properties properties;
    unordered_set<string> spectraIds;
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;

    void notifySpectrumHolderListeners(bool isAdd, list<ISpectrum*> spectra);

public:
    SpectralCluster( ICluster* copied,IConsensusSpectrumBuilder* consensusSpectrumBuilder);

    SpectralCluster(string id, IConsensusSpectrumBuilder* consensusSpectrumBuilder);

    unordered_set<string> getSpectralIds();

    string getMethodName();

    string getSpectralId();

    string getId();

    float getPrecursorMz();

    int getPrecursorCharge();

    ISpectrum* getConsensusSpectrum();

    list<ISpectrum*> getClusteredSpectra() const;

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();

    void addSpectrumHolderListener(SpectrumHolderListener* added) ;
    void removeSpectrumHolderListener(SpectrumHolderListener* removed);

    int getClusteredSpectraCount();

    string getProperty(string key);

    Properties getProperties();

    void setId(string id);

    void setProperty(string key,string value);

    void addSpectra(const ISpectrum* merged);

    void addSpectra(const list<ISpectrum*>& spectra);

    void removeSpectra(const ISpectrum* removed);

    void removeSpectra(const list<ISpectrum*> &spectra) ;

    bool isRemovedSupported();

    bool storesPeakLists();

    string toString();


    ~SpectralCluster();



};


#endif //CSPECTRA_CLUSTER_SPECTRALCLUSRER_H

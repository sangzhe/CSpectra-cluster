//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_SPECTRALCLUSRER_H
#define CSPECTRA_CLUSTER_SPECTRALCLUSRER_H


#include "ICluster.h"
#include "../Property/Properties.h"
#include "algorithm"
#include "../Spectrum/Spectrum.h"

class SpectralCluster: public ICluster{
private:
    PointerPool* pointer_pool = PoolFactory::getInstance();
    static string MethodName;

    string id = "";

    list<SpectrumHolderListener*> spectrumHolderListeners;

    vector<ISpectrum*> clusteredSpectra;

    Properties properties;
    list<string> spectralIds;
    IConsensusSpectrumBuilder* consensusSpectrumBuilder;

    void notifySpectrumHolderListeners(bool isAdd, vector<ISpectrum*> spectra);

public:
    SpectralCluster( ICluster* copied,IConsensusSpectrumBuilder* consensusSpectrumBuilder);

    SpectralCluster(string id, IConsensusSpectrumBuilder* consensusSpectrumBuilder);

    list<string> getSpectralIds();

    string getMethodName();

    string getSpectralId();

    string getId();

    float getPrecursorMz();

    int getPrecursorCharge();

    ISpectrum* getConsensusSpectrum();

    vector<ISpectrum*> getClusteredSpectra() ;

    IConsensusSpectrumBuilder* getConsensusSpectrumBuilder();

    void addSpectrumHolderListener(SpectrumHolderListener* added) ;
    void removeSpectrumHolderListener(SpectrumHolderListener* removed);

    int getClusteredSpectraCount();

    string getProperty(string key);

    Properties getProperties();

    void setId(string id);

    void setProperty(string key,string value);

    void addSpectra( ISpectrum* merged);

    void addSpectra( vector<ISpectrum*>& spectra);

    void removeSpectra( ISpectrum* removed);

    void removeSpectra( vector<ISpectrum*> &spectra) ;

    bool isRemovedSupported();

    bool storesPeakLists();

    string toString();

    bool isKnownComparisonMatch(string clusterId);

    list<ComparisonMatch> getComparisonMatches();


    ~SpectralCluster();



};


#endif //CSPECTRA_CLUSTER_SPECTRALCLUSRER_H

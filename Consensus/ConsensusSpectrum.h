//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H
#define CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H


#include "IConsensusSpectrumBuilder.h"
#include "ConsensusSpectrumBuilderFactory.h"
#include "../Cluster/ISpectrumHolder.h"
#include "../util/MZIntensityUtilities.h"
#include "ConsensusSpectrumFactory.h"


class ConsensusSpectrum: public IConsensusSpectrumBuilder {
public:
    ConsensusSpectrum() ;

    static  int DEFAULT_PEAKS_TO_KEEP;
    static  int SIZE_TO_ADD_EVERY_TIME;
    static  float NOISE_FILTER_INCREMENT;
    static  float FRACTION_OF_LOWEST_PEAK_TOKEEP;
    static ConsensusSpectrumBuilderFactory* FACTORY;
    static ConsensusSpectrumBuilderFactory* buildFactory();

    ConsensusSpectrum(string& id,const  int& nSpectra,const float& sumPrecusorMz,const float& sumPrecursorIntens,const int& sumCharge,const list<IPeak*>& peaks) ;

    void addSpectra(const ISpectrum* merged) ;

    void removeSpectra(const ISpectrum* removed) ;

    void onSpectraAdd(ISpectrumHolder* holder,list<ISpectrum*>& added);

    void onSpectraRemove(ISpectrumHolder* holder,list<ISpectrum*>& removed);

    bool isRemovedSupported();

    ISpectrum* getConsensusSpectrum() ;

    ISpectrum* internalGetConcensusSpectrum()const ;
    void clear();

    int getSpectraCount()const ;

//    string getName();
//
//    string getCurrentVersion();

    int getSumCharge()const ;

    double getSumPrecursorMz()const ;

    double getSumPrecursorIntensity()const ;

    list<IPeak*> getInternalPeaks()const ;

    void addPeaks(const list<IPeak*>& peaksToAdd);

    void removePeaks(const list<IPeak*> &peaksToRemove);

    void storeHeldPeaks(const list<IPeak*>& peaksToAdd);

    void addHeldPeaks();

    void internalAddPeaks(const list<IPeak*>& peaksToAdd);

    void update();

    static list<IPeak*> findConsensusPeaks(const list<IPeak*>& input,const int& peaksToKeep, int& nSpectra) ;

    bool isDirty;

    void addSpectra(const list<ISpectrum*> &spectra);

    void removeSpectra(const list<ISpectrum*> &spectra);

    void addSpectrumHolderListener(SpectrumHolderListener* added);
    void removeSpectrumHolderListener(SpectrumHolderListener* removed);

    string getMethodName();

//    IConsensusSpectrumBuilder& operator=(IConsensusSpectrumBuilder& O);



    ~ConsensusSpectrum();







private:
    string id;
    list<IPeak*> allPeaks;
    unordered_set<IPeak*> heldPeaks;
    list<IPeak*> consensusPeaks;


    ConsensusSpectrum(string& id);




protected:
    int nSpectra;
    bool Dirty;
    float averagePrecursorMz;
    float sumPrecursorMz;
    float averagePrecursorIntens;
    float sumPrecursorIntens;
    float lowestConcensusPeak;
    int averageCharge;
    int sumCharge;
    Spectrum* consensusSpectrum;
    const string methodName;
    const string methodVersion;
    static  float FINAL_MZ_THRESHOLD;
    static  float MZ_THRESHOLD_STEP;
    static  bool USE_ROUNDING;
    static  int MZ_PRECISION;
    void setDirty(bool isDirty) ;

    static list<IPeak*> mergeIdenticalPeaks(const list<IPeak*>& inPeaks);

    static list<IPeak*> adaptPeakIntensities(const list<IPeak*>& inp, int nSpectra);

    static list<IPeak*> filterNoise(const list<IPeak*>& inp);

    list<SpectrumHolderListener*> listeners;














};


#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H

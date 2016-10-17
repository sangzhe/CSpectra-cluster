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
#include "../Spectrum/Spectrum.h"


class ConsensusSpectrum: public IConsensusSpectrumBuilder {
public:
    ConsensusSpectrum() ;

    static  int DEFAULT_PEAKS_TO_KEEP;
    static  int SIZE_TO_ADD_EVERY_TIME;
    static  float NOISE_FILTER_INCREMENT;
    static  float FRACTION_OF_LOWEST_PEAK_TOKEEP;
    static ConsensusSpectrumBuilderFactory* FACTORY;
    static ConsensusSpectrumBuilderFactory* buildFactory();

    ConsensusSpectrum(string& id,const  int& nSpectra,const float& sumPrecusorMz,const float& sumPrecursorIntens,const int& sumCharge,const vector<Peak>& peaks) ;

    void addSpectra( ISpectrum* merged) ;

    void removeSpectra( ISpectrum* removed) ;

    void onSpectraAdd(ISpectrumHolder* holder,vector<ISpectrum*>& added);

    void onSpectraRemove(ISpectrumHolder* holder,vector<ISpectrum*>& removed);

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

    vector<Peak> getInternalPeaks()const ;

    void addPeaks(const vector<Peak>& peaksToAdd);

    void removePeaks(const vector<Peak> &peaksToRemove);

    void storeHeldPeaks(const vector<Peak>& peaksToAdd);

    void addHeldPeaks();

    void internalAddPeaks(const vector<Peak>& peaksToAdd);

    void update();

    static vector<Peak> findConsensusPeaks(const vector<Peak>& input,const int& peaksToKeep, int& nSpectra) ;


    void addSpectra(const vector<ISpectrum*> &spectra);

    void removeSpectra(const vector<ISpectrum*> &spectra);

    void addSpectrumHolderListener(SpectrumHolderListener* added);
    void removeSpectrumHolderListener(SpectrumHolderListener* removed);

    string getMethodName();

//    IConsensusSpectrumBuilder& operator=(IConsensusSpectrumBuilder& O);



    ~ConsensusSpectrum();







private:
    static PointerPool* pointer_pool;
    string id ="";
    vector<Peak> allPeaks;
    unordered_set<Peak> heldPeaks;
    vector<Peak> consensusPeaks;


    ConsensusSpectrum(string& id);




protected:
    int nSpectra = 0;
    bool Dirty = false;
    float averagePrecursorMz = 0;
    float sumPrecursorMz = 0;
    float averagePrecursorIntens = 0;
    float sumPrecursorIntens = 0;
    float lowestConcensusPeak = 0;
    int averageCharge = 0;
    int sumCharge = 0;
    Spectrum* consensusSpectrum = nullptr;
    const string methodName;
    const string methodVersion;
    static  float FINAL_MZ_THRESHOLD;
    static  float MZ_THRESHOLD_STEP;
    static  bool USE_ROUNDING;
    static  int MZ_PRECISION;
    bool isDirty();
    void setDirty(bool Dirty) ;

    static vector<Peak> mergeIdenticalPeaks(const vector<Peak>& inPeaks);

    static vector<Peak> adaptPeakIntensities(const vector<Peak>& inp, int nSpectra);

    static vector<Peak> filterNoise(const vector<Peak>& inp);

    list<SpectrumHolderListener*> listeners;














};


#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H

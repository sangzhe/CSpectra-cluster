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

    ConsensusSpectrum(string& id,const  int& nSpectra,const float& sumPrecusorMz,const float& sumPrecursorIntens,const int& sumCharge,const list<Peak>& peaks) ;

    void addSpectra(const Spectrum& merged) ;

    void removeSpectra(const Spectrum& removed) ;

    bool isRemovedSupported();

    Spectrum getConsensusSpectrum() ;

    Spectrum internalGetConcensusSpectrum()const ;
    void clear();

    int getSpectraCount()const ;

//    string getName();
//
//    string getCurrentVersion();

    int getSumCharge()const ;

    double getSumPrecursorMz()const ;

    double getSumPrecursorIntensity()const ;

    list<Peak> getInternalPeaks()const ;

    void addPeaks(const list<Peak>& peaksToAdd);

    void removePeaks(const list<Peak> &peaksToRemove);

    void storeHeldPeaks(const list<Peak>& peaksToAdd);

    void addHeldPeaks();

    void internalAddPeaks(const list<Peak>& peaksToAdd);

    void update();

    static list<Peak> findConsensusPeaks(const list<Peak>& input,const int& peaksToKeep, int& nSpectra) ;

    bool isDirty;

    void addSpectra(const list<Spectrum> &spectra);

    void removeSpectra(const list<Spectrum> &spectra);

    string getMethodName();

//    IConsensusSpectrumBuilder& operator=(IConsensusSpectrumBuilder& O);



    ~ConsensusSpectrum();







private:
    string id;
    list<Peak> allPeaks;
    unordered_set<Peak> heldPeaks;
    list<Peak> consensusPeaks;


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

    static list<Peak> mergeIdenticalPeaks(const list<Peak>& inPeaks);

    static list<Peak> adaptPeakIntensities(const list<Peak>& inp, int nSpectra);

    static list<Peak> filterNoise(const list<Peak>& inp);














};


#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H

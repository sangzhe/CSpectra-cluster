//
// Created by SangZhe on 16/8/31.
//

#ifndef CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H
#define CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H


#include <set>
#include "IConsensusSpectrumBuilder.h"
#include "ConsensusSpectrumBuilderFactory.h"
#include "ISpectrumHolder.h"
#include "ConsensusSpectrumFactory.h"

class ConsensusSpectrum: public IConsensusSpectrumBuilder {
public:
    ConsensusSpectrum();

    static  int DEFAULT_PEAKS_TO_KEEP;
    static  int SIZE_TO_ADD_EVERY_TIME;
    static  float NOISE_FILTER_INCREMENT;
    static  float FRACTION_OF_LOWEST_PEAK_TOKEEP;
    static ConsensusSpectrumBuilderFactory* FACTORY;
    static ConsensusSpectrumBuilderFactory* buildFactory();

    ConsensusSpectrum(string& id, int& nSpectra,float& sumPrecusorMz,float& sumPrecursorIntens,int& sumCharge,list<IPeak*>& peaks);

    void addSpectra(ISpectrum& merged);

    void removeSpectra(ISpectrum& removed);

    bool isRemovedSupported();

    ISpectrum* getConsensusSpectrum();

    ISpectrum* internalGetConcensusSpectrum();
    void clear();

    int getSpectraCount();

//    string getName();
//
//    string getCurrentVersion();

    int getSumCharge();

    double getSumPrecursorMz();

    double getSumPrecursorIntensity();

    list<IPeak*> getInternalPeaks();

    void addPeaks(list<IPeak*>& peaksToAdd);

    void removePeaks(list<IPeak*> &peaksToRemove);

    void storeHeldPeaks(list<IPeak*>& peaksToAdd);

    void addHeldPeaks();

    void internalAddPeaks(list<IPeak*>& peaksToAdd);

    void update();

    static list<IPeak*> findConsensusPeaks(list<IPeak*>& input,const int& peaksToKeep, int& nSpectra);

    bool isDirty;

    void addSpectra(list<ISpectrum*> &spectra);

    void removeSpectra(list<ISpectrum*> &spectra);

    ~ConsensusSpectrum();






private:
     string id;

    list<IPeak*> allPeaks;
    set<IPeak*> heldPeaks;
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
    ISpectrum* consensusSpectrum;
    const string methodName;
    const string methodVersion;
    static  float FINAL_MZ_THRESHOLD;
    static  float MZ_THRESHOLD_STEP;
    static  bool USE_ROUNDING;
    static  int MZ_PRECISION;
    void setDirty(bool isDirty);

    static list<IPeak*> mergeIdenticalPeaks(list<IPeak*>& inPeaks);

    static list<IPeak*> adaptPeakIntensities(list<IPeak*>& inp, int nSpectra);

    static list<IPeak*> filterNoise(list<IPeak*>& inp);














};


#endif //CSPECTRA_CLUSTER_CONSENSUSSPECTRUM_H

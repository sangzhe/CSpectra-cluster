//
// Created by SangZhe on 16/9/9.
//

#ifndef CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H
#define CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H


#include "IConsensusSpectrumBuilder.h"
#include "ConsensusSpectrumBuilderFactory.h"
#include "GreedyConsensusSpectrumFactory.h"
#include "../util/Function/peak/BinnedHighestNPeakFunction.h"
#include "../Spectrum/Spectrum.h"

class GreedyConsensusSpectrumFactory;

class GreedyConsensusSpectrum : public IConsensusSpectrumBuilder{
public:
    static  int DEFAULT_PEAKS_TO_KEEP ;

    static  float NOISE_FILTER_INCREMENT;

    static GreedyConsensusSpectrumFactory *FACTORY;

    static ConsensusSpectrumBuilderFactory* buildFactory();

    GreedyConsensusSpectrum(float fragmentTolerance);

    GreedyConsensusSpectrum(float fragmentTolerance, string id);

    GreedyConsensusSpectrum(float fragmentTolerance, string id, int nSpectra, double sumPrecursorMz, double sumPrecursorIntens, int sumCharge,const vector<Peak> peaks);

    void addSpectra( ISpectrum* merged);
    void addSpectra(const vector<ISpectrum*>& spectra);
    bool isRemovedSupported();
    void removeSpectra( ISpectrum* removed);
    void removeSpectra(const vector<ISpectrum*> &spectra);
    void addConsensusSpectrum( IConsensusSpectrumBuilder& consensusSpectrumToAdd);

    ISpectrum* getConsensusSpectrum() ;

    void addSpectrumHolderListener(SpectrumHolderListener* added);
    void removeSpectrumHolderListener(SpectrumHolderListener* removed);

    void onSpectraAdd(ISpectrumHolder* holder,vector<ISpectrum*>& added);

    void onSpectraRemove(ISpectrumHolder* holder,vector<ISpectrum*>& removed);

    void clear();

    int getSpectraCount()const;
    int getSumCharge()const;
    double getSumPrecursorMz()const;
    double getSumPrecursorIntensity()const;

    string getMethodName();
    ~GreedyConsensusSpectrum();


protected:
    float fragmentTolerance;
    int nSpectra = 0;
    float averagePrecursorMz;
    double sumPrecursorMz =0;
    float averagePrecursorIntens;
    double sumPrecursorIntens = 0;
    int averageCharge;
    int sumCharge = 0;
    Spectrum* consensusSpectrum = nullptr;

    list<SpectrumHolderListener*> listeners;

    string methodName = "GreedyConsensusSpectrum";
    string methodVersion = "0.1";

    void updateConsensusSpectrum();

    void addPeaksToConsensus(const vector<Peak>& peaksToAdd);

    void updateProperties();

    static vector<Peak> findConsensusPeaks(const vector<Peak>& input, int nSpectra);

    static vector<Peak> filterNoise( const vector<Peak>& inp);

    static vector<Peak> adaptPeakIntensities(const vector<Peak>& inp, int nSpectra);

    vector<Peak> mergeIdenticalPeaks(const vector<Peak>& inPeaks)const;

    bool isDirty() ;

    void setIsDirty(bool isDirty);

private:
    static PointerPool* pointer_pool;
    string id;
    bool Dirty = false;
    vector<Peak> consensusPeaks;







};


#endif //CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H

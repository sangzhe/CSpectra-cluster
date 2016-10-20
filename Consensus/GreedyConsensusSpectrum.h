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
    void addSpectra( vector<ISpectrum*>& spectra);
    bool isRemovedSupported();
    void removeSpectra( ISpectrum* removed);
    void removeSpectra( vector<ISpectrum*> &spectra);
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
    float fragmentTolerance = 0;
    int nSpectra = 0;
    float averagePrecursorMz = 0;
    double sumPrecursorMz =0;
    float averagePrecursorIntens = 0;
    double sumPrecursorIntens = 0;
    int averageCharge = 0;
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
    string id = "";
    bool Dirty = true;
    vector<Peak> consensusPeaks;







};


#endif //CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H

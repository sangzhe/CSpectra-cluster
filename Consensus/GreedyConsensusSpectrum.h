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

    GreedyConsensusSpectrum(float fragmentTolerance, string id, int nSpectra, double sumPrecursorMz, double sumPrecursorIntens, int sumCharge,const vector<IPeak*> peaks);

    void addSpectra(const ISpectrum* merged);
    void addSpectra(const vector<ISpectrum*>& spectra);
    bool isRemovedSupported();
    void removeSpectra(const ISpectrum* removed);
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


protected:
    float fragmentTolerance;
    int nSpectra;
    float averagePrecursorMz;
    double sumPrecursorMz;
    float averagePrecursorIntens;
    double sumPrecursorIntens;
    int averageCharge;
    int sumCharge;
    Spectrum* consensusSpectrum;

    list<SpectrumHolderListener*> listeners;

    string methodName = "GreedyConsensusSpectrum";
    string methodVersion = "0.1";

    void updateConsensusSpectrum();

    void addPeaksToConsensus(const vector<IPeak*>& peaksToAdd);

    void updateProperties();

    static vector<IPeak*> findConsensusPeaks(const vector<IPeak*>& input, int nSpectra);

    static vector<IPeak*> filterNoise( const vector<IPeak*>& inp);

    static vector<IPeak*> adaptPeakIntensities(const vector<IPeak*>& inp, int nSpectra);

    vector<IPeak*> mergeIdenticalPeaks(const vector<IPeak*>& inPeaks)const;

    bool isDirty() ;

    void setIsDirty(bool isDirty);

private:
    static PointerPool* pointer_pool;
    string id;
    bool Dirty = true;
    vector<IPeak*> consensusPeaks;







};


#endif //CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H

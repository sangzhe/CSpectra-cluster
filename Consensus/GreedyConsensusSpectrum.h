//
// Created by SangZhe on 16/9/9.
//

#ifndef CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H
#define CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H


#include "IConsensusSpectrumBuilder.h"
#include "ConsensusSpectrumBuilderFactory.h"
#include "GreedyConsensusSpectrumFactory.h"
#include "../util/Function/peak/BinnedHighestNPeakFunction.h"

class GreedyConsensusSpectrumFactory;

class GreedyConsensusSpectrum : public IConsensusSpectrumBuilder{
public:
    static  int DEFAULT_PEAKS_TO_KEEP ;

    static  float NOISE_FILTER_INCREMENT;

    static GreedyConsensusSpectrumFactory *FACTORY;

    static ConsensusSpectrumBuilderFactory* buildFactory();

    GreedyConsensusSpectrum(float fragmentTolerance);

    GreedyConsensusSpectrum(float fragmentTolerance, string id);

    GreedyConsensusSpectrum(float fragmentTolerance, string id, int nSpectra, double sumPrecursorMz, double sumPrecursorIntens, int sumCharge,const list<Peak> peaks);

    void addSpectra(const Spectrum& merged);
    void addSpectra(const list<Spectrum>& spectra);
    bool isRemovedSupported();
    void removeSpectra(const Spectrum& removed);
    void removeSpectra(const list<Spectrum> &spectra);
    void addConsensusSpectrum( IConsensusSpectrumBuilder& consensusSpectrumToAdd);

    Spectrum getConsensusSpectrum() ;


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

    string methodName = "GreedyConsensusSpectrum";
    string methodVersion = "0.1";

    void updateConsensusSpectrum();

    void addPeaksToConsensus(const list<Peak>& peaksToAdd);

    void updateProperties();

    static list<Peak> findConsensusPeaks(const list<Peak>& input, int nSpectra);

    static list<Peak> filterNoise( const list<Peak>& inp);

    static list<Peak> adaptPeakIntensities(const list<Peak>& inp, int nSpectra);

    list<Peak> mergeIdenticalPeaks(const list<Peak>& inPeaks)const;

    bool isDirty() ;

    void setIsDirty(bool isDirty);

private:
    string id;
    bool Dirty = true;
    list<Peak> consensusPeaks;







};


#endif //CSPECTRA_CLUSTER_GREEDYCONSENSUSSPECTRUM_H

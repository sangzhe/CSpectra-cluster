//
// Created by SangZhe on 16/8/25.
//

#ifndef CSPECTRA_CLUSTER_DEFAULTS_H
#define CSPECTRA_CLUSTER_DEFAULTS_H


#include "../quality/IQualityScorer.h"
#include "../Consensus/IConsensusSpectrumBuilder.h"
#include "../Consensus/ConsensusSpectrumBuilderFactory.h"
#include "../Consensus/ConsensusSpectrum.h"
#include "../quality/SignalToNoiseChecker.h"
#include "Function/peak/HighestNPeakFunction.h"
#include "Function/spectrum/RemovePrecursorPeaksFunction.h"
#include "Function/spectrum/RemoveImpossiblyHighPeaksFunction.h"
#include "../similarity/ISimilarityChecker.h"
#include "../similarity/CombinedFisherIntensityTest.h"



class Defaults {
    public:
            static float DEFAULT_FRAGMENT_ION_TOLERANCE;
            static  double DEFAULT_SIMILARITY_THRESHOLD;
            static float getFragmentIonTolerance();
            static IQualityScorer* getDefaultQualityScorer();
            static ISpectrum* doDefaultPeakFilter(const ISpectrum& spectrum);
            static IConsensusSpectrumBuilder* getDefaultConsensusSpectrumBuilder();
            static ISimilarityChecker* getDefaultSimilarityChecker();
            static double getSimilarityThreshold();
            static void setFragmentIonTolerance(float fragmentIonTolerance);
            ~Defaults();

private:
            static float fragmentIonTolerance;
            static ISimilarityChecker* defaultSimilarityChecker;
            static IQualityScorer* defaultQualityScorer;
            static ConsensusSpectrumBuilderFactory* consensusFactory;
            static double similarityThreshold = DEFAULT_SIMILARITY_THRESHOLD;

};


#endif //CSPECTRA_CLUSTER_DEFAULTS_H

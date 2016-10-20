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
            static  int DEFAULT_MIN_NUMBER_COMPARISONS;
            static float getFragmentIonTolerance();
            static IQualityScorer* getDefaultQualityScorer();
            static ISpectrum* doDefaultPeakFilter( ISpectrum& spectrum);
            static IConsensusSpectrumBuilder* getDefaultConsensusSpectrumBuilder();
            static ISimilarityChecker* getDefaultSimilarityChecker();
            static double getSimilarityThreshold();
            static void setFragmentIonTolerance(float fragmentIonTolerance);
            static int getMinNumberComparisons();
            ~Defaults();

private:
            static float fragmentIonTolerance;
            static int minNumberComparisons;
            static ISimilarityChecker* defaultSimilarityChecker;
            static IQualityScorer* defaultQualityScorer;
            static ConsensusSpectrumBuilderFactory* consensusFactory;
            static double similarityThreshold ;

};


#endif //CSPECTRA_CLUSTER_DEFAULTS_H

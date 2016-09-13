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



class Defaults {
    public:
            static float DEFAULT_FRAGMENT_ION_TOLERANCE;
            static float getFragmentIonTolerance();
            static IQualityScorer* getDefaultQualityScorer();
            static Spectrum doDefaultPeakFilter(Spectrum& spectrum);
            static IConsensusSpectrumBuilder* getDefaultConsensusSpectrumBuilder();
            ~Defaults();
    private:
            static float fragmentIonTolerance;

            static IQualityScorer* defaultQualityScorer;
            static ConsensusSpectrumBuilderFactory* consensusFactory;

};


#endif //CSPECTRA_CLUSTER_DEFAULTS_H

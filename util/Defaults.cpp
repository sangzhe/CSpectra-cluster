//
// Created by SangZhe on 16/8/25.
//

#include "Defaults.h"

float Defaults::DEFAULT_FRAGMENT_ION_TOLERANCE = 0.5F;

float Defaults::fragmentIonTolerance = DEFAULT_FRAGMENT_ION_TOLERANCE;

ConsensusSpectrumBuilderFactory* Defaults::consensusFactory = new ConsensusSpectrumFactory();

IQualityScorer* Defaults::defaultQualityScorer = new SignalToNoiseChecker();

IConsensusSpectrumBuilder* Defaults::getDefaultConsensusSpectrumBuilder() {
    return consensusFactory->getConsensusSpectrumBuilder();

}

IQualityScorer* Defaults::getDefaultQualityScorer() {
    return defaultQualityScorer;

}

Spectrum Defaults::doDefaultPeakFilter(Spectrum& spectrum) {
    Spectrum new_spec = spectrum;

    RemovePrecursorPeaksFunction func1 = RemovePrecursorPeaksFunction(fragmentIonTolerance);
    func1.apply(new_spec);

    list<Peak> new_peak = spectrum.getPeaks();
    HighestNPeakFunction func2 = HighestNPeakFunction(150);
    func2.apply(new_peak);
    Spectrum ret(new_spec,new_peak);

    RemoveImpossiblyHighPeaksFunction func3 = RemoveImpossiblyHighPeaksFunction();
    func3.apply(ret);

    return ret;
}

float Defaults::getFragmentIonTolerance() {
    return fragmentIonTolerance;
}

Defaults::~Defaults() {
    delete consensusFactory,defaultQualityScorer;
}
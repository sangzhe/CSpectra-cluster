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

void Defaults::doDefaultPeakFilter(ISpectrum* spectrum) {
    RemovePrecursorPeaksFunction func1 = RemovePrecursorPeaksFunction(fragmentIonTolerance);
    func1.apply(spectrum);

    list<IPeak*> new_peak = spectrum->getPeaks();
    HighestNPeakFunction func2 = HighestNPeakFunction(150);
    func2.apply(new_peak);
    ISpectrum* new_spectrum = new Spectrum(*spectrum,new_peak);

    RemoveImpossiblyHighPeaksFunction func3 = RemoveImpossiblyHighPeaksFunction();
    func3.apply(new_spectrum);
    spectrum = new_spectrum;
}

Defaults::~Defaults() {
    delete consensusFactory,defaultQualityScorer;
}
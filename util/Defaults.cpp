//
// Created by SangZhe on 16/8/25.
//

#include "Defaults.h"

float Defaults::DEFAULT_FRAGMENT_ION_TOLERANCE = 0.5F;

float Defaults::fragmentIonTolerance = DEFAULT_FRAGMENT_ION_TOLERANCE;

double Defaults::DEFAULT_SIMILARITY_THRESHOLD = 0.99;

int Defaults::DEFAULT_MIN_NUMBER_COMPARISONS = 0;

int Defaults::minNumberComparisons = DEFAULT_MIN_NUMBER_COMPARISONS;


double Defaults::similarityThreshold = DEFAULT_SIMILARITY_THRESHOLD;


ConsensusSpectrumBuilderFactory* Defaults::consensusFactory = new ConsensusSpectrumFactory();

IQualityScorer* Defaults::defaultQualityScorer = new SignalToNoiseChecker();

ISimilarityChecker* Defaults::defaultSimilarityChecker = new CombinedFisherIntensityTest((float)getFragmentIonTolerance());


ISimilarityChecker* Defaults::getDefaultSimilarityChecker() {
    return defaultSimilarityChecker;
}

IConsensusSpectrumBuilder* Defaults::getDefaultConsensusSpectrumBuilder() {
    return consensusFactory->getConsensusSpectrumBuilder();

}

IQualityScorer* Defaults::getDefaultQualityScorer() {
    return defaultQualityScorer;

}

ISpectrum* Defaults::doDefaultPeakFilter(const ISpectrum& spectrum) {
    RemovePrecursorPeaksFunction func1 = RemovePrecursorPeaksFunction(fragmentIonTolerance);
    ISpectrum* spec1 = func1.apply(spectrum);

    vector<IPeak*> new_peak = spec1->getPeaks();
    HighestNPeakFunction func2 = HighestNPeakFunction(150);
    new_peak = func2.apply(new_peak);
    ISpectrum* spec2 = new Spectrum(*spec1,new_peak);

    RemoveImpossiblyHighPeaksFunction func3 = RemoveImpossiblyHighPeaksFunction();
    ISpectrum *spec3 = func3.apply(*spec2);
    delete spec1,spec2;
    return spec3;
}

double Defaults::getSimilarityThreshold() {
    return similarityThreshold;
}
float Defaults::getFragmentIonTolerance() {
    return fragmentIonTolerance;
}
void Defaults::setFragmentIonTolerance(float fragmentIonTolerance) {
    Defaults::fragmentIonTolerance = fragmentIonTolerance;
}

int Defaults::getMinNumberComparisons() {
    return minNumberComparisons;
}
Defaults::~Defaults() {
    delete consensusFactory,defaultQualityScorer;
}
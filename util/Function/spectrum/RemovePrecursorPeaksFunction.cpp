//
// Created by SangZhe on 16/9/7.
//

#include "RemovePrecursorPeaksFunction.h"

RemovePrecursorPeaksFunction::RemovePrecursorPeaksFunction(float fragmentIonTolerance) {
    this->fragmentIonTolerance = fragmentIonTolerance;
}

bool RemovePrecursorPeaksFunction::isWithinRange(float min, float max, float value) {
    return (value >= min && value <= max);
}

void RemovePrecursorPeaksFunction::apply(ISpectrum *o) {
    // calculate m/z of neutral losses
     float floatCharge     = (float) o->getPrecursorCharge();
     float waterLoss       = o->getPrecursorMz() - (Mass::WATER_MONO / floatCharge);
     float doubleWaterLoss = o->getPrecursorMz() - (2.0F * Mass::WATER_MONO / floatCharge);
     float ammoniumLoss    = o->getPrecursorMz() - (Mass::AMMONIA_MONO / floatCharge);

    // calculate range based on fragmentIonTolerance
     float minWaterLoss        = waterLoss - fragmentIonTolerance;
     float maxWaterLoss        = waterLoss + fragmentIonTolerance;
     float minDoubleWaterLoss  = doubleWaterLoss - fragmentIonTolerance;
     float maxDoubleWaterLoss  = doubleWaterLoss + fragmentIonTolerance;
     float minAmmoniumLoss     = ammoniumLoss - fragmentIonTolerance;
     float maxAmmoniumLoss     = ammoniumLoss + fragmentIonTolerance;

    // also filter the default precursor
     float minPrecursor = o->getPrecursorMz() - fragmentIonTolerance;
     float maxPrecursor = o->getPrecursorCharge() + fragmentIonTolerance;

    list<IPeak*> filteredPeakList;
    list<IPeak*> peak = o->getPeaks();
    list<IPeak*>::iterator iterator1;
    for(iterator1 = peak.begin();iterator1 != peak.end();iterator1++) {
        float peakMz = (*iterator1)->getMz();
        // ignore any peak that could be a neutral loss
        if (isWithinRange(minWaterLoss, maxWaterLoss, peakMz))
            continue;
        if (isWithinRange(minDoubleWaterLoss, maxDoubleWaterLoss, peakMz))
            continue;
        if (isWithinRange(minAmmoniumLoss, maxAmmoniumLoss, peakMz))
            continue;
        if (isWithinRange(minPrecursor, maxPrecursor, peakMz))
            continue;

        filteredPeakList.push_back(*iterator1);
    }
    ISpectrum* filteredSpectrum = new Spectrum(*o, filteredPeakList, true);
    delete o;
    o = filteredSpectrum;
}
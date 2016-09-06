//
// Created by SangZhe on 16/8/25.
//

#include "Spectrum.h"

int Spectrum::BAD_QUALITY_MEASURE = -1;

int currentMAjorPeakCount = 0;

Spectrum::Spectrum(string &pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer,
                   list<IPeak*> &inpeaks) {
    this->id = pId;
    this->precursorCharge = pPrecursorCharge;
    this->precursorMz = pPrecursorMz;
    this->qualityScorer = qualityScorer;
    this->peaks.clear();
    peaks.merge(inpeaks);
    peaks.sort(Peak::cmpPeakMz);
    calculateIntensities();
}

Spectrum::Spectrum(ISpectrum& spectrum){
    list<IPeak*> peaks = spectrum.getPeaks();
    new (this)Spectrum(spectrum,peaks);
}

Spectrum::Spectrum(ISpectrum& spectrum, list<IPeak*>& inpeaks){
    new (this)Spectrum(spectrum,inpeaks,false);
}

Spectrum::Spectrum(ISpectrum& spectrum, list<IPeak*>& inpeaks, bool isSortedList) {
    this->id = spectrum.getId();
    this->precursorCharge = spectrum.getPrecursorCharge();
    this->precursorMz = spectrum.getPrecursorMz();
    this->qualityScorer = spectrum.getQualityScorer();

    peaks.clear();
    peaks.merge(inpeaks);

    if(!isSortedList)
        peaks.sort(Peak::cmpPeakMz);

    Properties* props = spectrum.getProperties();
    if(props != NULL){
        properties->putAll(*props);
    }
    calculateIntensities();
}

void Spectrum::calculateIntensities() {
    double totalIntensityX = 0;
    double sumSquareIntensityX = 0;
    list<IPeak*>::iterator iter;
    for(iter = peaks.begin();iter != peaks.end();iter++){
        double intensity = (*iter)->getIntensity();
        totalIntensityX += intensity;
        double ji = convertIntensity(*(*iter));
        sumSquareIntensityX += ji *ji;
    }
    totalIntensity = totalIntensityX;
    sumSquareIntensity = sumSquareIntensityX;
}

double Spectrum::convertIntensity(IPeak &p1) {
    double intensity = p1.getIntensity();
    if(intensity == 0) return 0;
    return 1 + log(intensity) ;
}



ISpectrum* Spectrum::getHighestNPeaks(int numberRequested) {
    ISpectrum* ret = highestPeaks.at(numberRequested);
    if (ret == NULL){
        ret = buildHighestPeaks(numberRequested);
        int numberPeaks = ret->getPeaksCount();
        for(int i = numberPeaks;i >= numberPeaks;i--){
            highestPeaks.insert(map<int,ISpectrum*>::value_type(i,ret));
        }
    }
    return ret;

}

ISpectrum* Spectrum::buildHighestPeaks(int numberRequested) {
    list<IPeak*> byIntensity = this->getPeaks();
    byIntensity.sort(Peak::cmpPeakIntensity);
    list<IPeak*>* holder = new list<IPeak*>;
    list<IPeak*>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        holder->push_back(*iter);
        if(holder->size() >= numberRequested) break;
    }
    Spectrum* ret = new Spectrum(*this,*holder);
    delete holder;
    return ret;
}

double Spectrum::getQualityScore() {
    if(qualityMeasure == BAD_QUALITY_MEASURE){
        qualityMeasure = qualityScorer->calculateQualityScore(*this);
    }
    return qualityMeasure;
}

IQualityScorer* Spectrum::getQualityScorer() {
    return qualityScorer;
}

int Spectrum::getPrecursorCharge() {
    return precursorCharge;

}

string Spectrum::getId() {
    return id;
}

list<IPeak*> Spectrum::getPeaks() {
    const list<IPeak*> ret = peaks;
    return ret;
}

int Spectrum::getPeaksCount() {
    return peaks.size();
}


float Spectrum::getPrecursorMz() {
    return precursorMz;
}

Properties* Spectrum::getProperties() {
    return properties;
}

string Spectrum::getProperty(string key) {
    return properties->getProperty(key);
}

void Spectrum::setProperty(string key, string value) {
    properties->setProperty(key,value);
}

double Spectrum::getTotalIntensity() {
    return  totalIntensity;
}

double Spectrum::getSumSquareIntensity() {
    return sumSquareIntensity;
}

Spectrum::~Spectrum() {
    delete properties,qualityScorer;
}



//
// Created by SangZhe on 16/8/25.
//

#include "Spectrum.h"
#include "../util/IOUtilities.h"

int Spectrum::BAD_QUALITY_MEASURE = -1;

int currentMAjorPeakCount = 0;

Spectrum::Spectrum() {};

Spectrum::Spectrum(string &pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer,
                   const list<Peak> &inpeaks) {
    this->id = pId;
    this->precursorCharge = pPrecursorCharge;
    this->precursorMz = pPrecursorMz;
    this->qualityScorer = qualityScorer;
    this->peaks.clear();
    peaks.merge(list<Peak>(inpeaks));
    peaks.sort(Peak::cmpPeakMz);
    calculateIntensities();
}

Spectrum::Spectrum( const Spectrum& spectrum){
    list<Peak> peaks = spectrum.getPeaks();
    new (this)Spectrum(spectrum,peaks);
}

Spectrum::Spectrum( const Spectrum& spectrum,  const list<Peak>& inpeaks){
    new (this)Spectrum(spectrum,inpeaks,false);
}

Spectrum::Spectrum( const Spectrum& spectrum,  const list<Peak>& inpeaks, bool isSortedList) {
    this->id = spectrum.getId();
    this->precursorCharge = spectrum.getPrecursorCharge();
    this->precursorMz = spectrum.getPrecursorMz();
    this->qualityScorer = spectrum.getQualityScorer();

    peaks.clear();
    peaks.merge(list<Peak>(inpeaks));

    if(!isSortedList)
        peaks.sort(Peak::cmpPeakMz);

    Properties props = spectrum.getProperties();
    properties.putAll(props);
    calculateIntensities();
}

void Spectrum::calculateIntensities() {
    double totalIntensityX = 0;
    double sumSquareIntensityX = 0;
    list<Peak>::iterator iter;
    for(iter = peaks.begin();iter != peaks.end();iter++){
        double intensity = iter->getIntensity();
        totalIntensityX += intensity;
        double ji = convertIntensity(*iter);
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



Spectrum Spectrum::getHighestNPeaks(int numberRequested) {

    Spectrum ret = highestPeaks.at(numberRequested);
    if (ret == Spectrum()){
        Spectrum replace(buildHighestPeaks(numberRequested));
        ret = replace;
        int numberPeaks = ret.getPeaksCount();
        for(int i = numberPeaks;i >= numberPeaks;i--){
            const int num = i;
            highestPeaks.insert(pair<int,Spectrum>(num,ret));
//            highestPeaks[num] = ret;
        }
    }
    return ret;

}

Spectrum Spectrum::buildHighestPeaks(int numberRequested) const {
    list<Peak> byIntensity = this->getPeaks();
    byIntensity.sort(Peak::cmpPeakIntensity);
    list<Peak> holder;
    list<Peak>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        holder.push_back(*iter);
        if(holder.size() >= numberRequested) break;
    }
    Spectrum ret(*this,holder);

    return ret;
}

double Spectrum::getQualityScore() {
    if(qualityMeasure == BAD_QUALITY_MEASURE){
        qualityMeasure = qualityScorer->calculateQualityScore(*this);
    }
    return qualityMeasure;
}

IQualityScorer* Spectrum::getQualityScorer() const {
    return qualityScorer;
}

int Spectrum::getPrecursorCharge() const{
    return precursorCharge;

}

string Spectrum::getId() const{
    return id;
}

list<Peak> Spectrum::getPeaks() const{
    return peaks;
}

int Spectrum::getPeaksCount() const{
    return peaks.size();
}


float Spectrum::getPrecursorMz() const{
    return precursorMz;
}

Properties Spectrum::getProperties() const{
    return properties;
}

string Spectrum::getProperty(string key) {
    return properties.getProperty(key);
}

void Spectrum::setProperty(string key, string value) {
    properties.setProperty(key,value);
}

double Spectrum::getTotalIntensity() const{
    return  totalIntensity;
}

double Spectrum::getSumSquareIntensity() const{
    return sumSquareIntensity;
}

bool Spectrum::operator<(const Spectrum &O) const{
    return (IOUtilities::compare(totalIntensity,O.totalIntensity) == -1);
}
bool Spectrum::operator==(const Spectrum& O) const{
    if (precursorCharge != O.getPrecursorCharge()) return false;
    if (IOUtilities::compare(O.getPrecursorMz(), precursorMz) != 0) return false;
    if ( id != (O.getId())) return false;
    if (peaks.size() != (O.getPeaks().size())) {
        return false;
    }
    list<Peak> peaks1 = peaks;
    list<Peak>::iterator iter1;
    list<Peak>::iterator iter2;
    iter1= peaks1.begin();
    iter2 = O.getPeaks().begin();

    for(iter1,iter2;iter1 != peaks1.end();iter1++,iter2++){
        Peak pk0 = *iter1;
        Peak pk1 = *iter2;
        if ( ! (pk0 == pk1)) {
            return false;
        }
    }
    return true;
}

Spectrum& Spectrum::operator=(const Spectrum& O) {
    this->properties = O.getProperties();
    this->currentMAjorPeakCount = O.getPeaksCount();
    this->totalIntensity = O.getTotalIntensity();
    this->peaks = O.getPeaks();
    this->precursorCharge = O.getPrecursorCharge();
    this->precursorMz = O.getPrecursorMz();
    this->sumSquareIntensity = O.getSumSquareIntensity();
    this->id = O.getId();
}

size_t hash_value(const Spectrum &p) {
    std::size_t seed = 0;
    boost::hash_combine(seed,p.precursorMz);
    boost::hash_combine(seed,p.precursorCharge);
    boost::hash_combine(seed,p.id);
    boost::hash_combine(seed,p.sumSquareIntensity);
    boost::hash_combine(seed,p.totalIntensity);
    return seed;
}





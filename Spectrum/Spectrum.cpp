//
// Created by SangZhe on 16/8/25.
//

#include "Spectrum.h"

int Spectrum::BAD_QUALITY_MEASURE = -1;

int currentMAjorPeakCount = 0;

Spectrum::Spectrum() {};

Spectrum::Spectrum(string &pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer,
                   const vector<IPeak*> &inpeaks) {
    this->id = pId;
    this->precursorCharge = pPrecursorCharge;
    this->precursorMz = pPrecursorMz;
    this->qualityScorer = qualityScorer;
    this->peaks.clear();
    vector<IPeak*> in = inpeaks;
    peaks.insert(peaks.end(),in.begin(),in.end());
    PointerPool::add(in);
    sort(peaks.begin(),peaks.end(),Peak::cmpPeakMz);
    calculateIntensities();
}

Spectrum::Spectrum( const ISpectrum& spectrum){
    vector<IPeak*> peaks = spectrum.getPeaks();
    new (this)Spectrum(spectrum,peaks);
}

Spectrum::Spectrum( const ISpectrum& spectrum,  const vector<IPeak*>& inpeaks){
    new (this)Spectrum(spectrum,inpeaks,false);
}

Spectrum::Spectrum( const ISpectrum& spectrum,  const vector<IPeak*>& inpeaks, bool isSortedList) {
    this->id = spectrum.getId();
    this->precursorCharge = spectrum.getPrecursorCharge();
    this->precursorMz = spectrum.getPrecursorMz();
    this->qualityScorer = spectrum.getQualityScorer();

    peaks.clear();
    peaks.insert(peaks.end(),inpeaks.begin(),inpeaks.end());
    PointerPool::add(inpeaks);


    if(!isSortedList)
        sort(peaks.begin(),peaks.end(),Peak::cmpPeakMz);

    Properties props = spectrum.getProperties();
    properties.putAll(props);
    calculateIntensities();
}

void Spectrum::calculateIntensities() {
    double totalIntensityX = 0;
    double sumSquareIntensityX = 0;
    vector<IPeak*>::iterator iter;
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
    ISpectrum *ret;
    try {
        ret = highestPeaks.at(numberRequested);
    }catch (out_of_range) {
        Spectrum *replace = new Spectrum(buildHighestPeaks(numberRequested));
        ret = replace;
        int numberPeaks = ret->getPeaksCount();
        for (int i = numberPeaks; i >= numberPeaks; i--) {
            const int num = i;
            highestPeaks.insert(pair<int, ISpectrum*>(num, ret));
            PointerPool::add(ret);
        }
    }
    return ret;

}

Spectrum Spectrum::buildHighestPeaks(int numberRequested) const {
    vector<IPeak*> byIntensity = this->getPeaks();
    sort(byIntensity.begin(),byIntensity.end(),Peak::cmpPeakIntensity);
    vector<IPeak*> holder;
    vector<IPeak*>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();iter++){
        holder.push_back(*iter);
        PointerPool::add(*iter);
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

vector<IPeak*> Spectrum::getPeaks() const{
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

//bool Spectrum::operator<(const Spectrum &O) const{
//    return (IOUtilities::compare(totalIntensity,O.totalIntensity) == -1);
//}
bool Spectrum::operator==(const ISpectrum& O) const{
    if (precursorCharge != O.getPrecursorCharge()) return false;
    if (IOUtilities::compare(O.getPrecursorMz(), precursorMz) != 0) return false;
    if ( id != (O.getId())) return false;
    if (peaks.size() != (O.getPeaks().size())) {
        return false;
    }
    vector<IPeak*> peaks1 = peaks;
    vector<IPeak*>::iterator iter1;
    vector<IPeak*>::iterator iter2;
    iter1= peaks1.begin();
    iter2 = O.getPeaks().begin();

    IPeak* pk0;
    IPeak* pk1;
    for(iter1,iter2;iter1 != peaks1.end();iter1++,iter2++){
        pk0 = *iter1;
        pk1 = *iter2;
        if ( ! (*pk0 == *pk1)) {
            PointerPool::remove(pk0);
            PointerPool::remove(pk1);
            return false;
        }
    }
    PointerPool::remove(pk0);
    PointerPool::remove(pk1);
    return true;
}

//Spectrum& Spectrum::operator=(const Spectrum& O) {
//    this->properties = O.getProperties();
//    this->currentMAjorPeakCount = O.getPeaksCount();
//    this->totalIntensity = O.getTotalIntensity();
//    this->peaks = O.getPeaks();
//    this->precursorCharge = O.getPrecursorCharge();
//    this->precursorMz = O.getPrecursorMz();
//    this->sumSquareIntensity = O.getSumSquareIntensity();
//    this->id = O.getId();
//}
//
//size_t hash_value(const Spectrum &p) {
//    std::size_t seed = 0;
//    boost::hash_combine(seed,p.precursorMz);
//    boost::hash_combine(seed,p.precursorCharge);
//    boost::hash_combine(seed,p.id);
//    boost::hash_combine(seed,p.sumSquareIntensity);
//    boost::hash_combine(seed,p.totalIntensity);
//    return seed;
//}

Spectrum::~Spectrum() {
    PointerPool::remove(peaks);
    unordered_map<int,ISpectrum*>::iterator iter;
    for(iter = highestPeaks.begin();iter != highestPeaks.end();iter++){
        PointerPool::remove(iter->second);
    }
}



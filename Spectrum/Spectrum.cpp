//
// Created by SangZhe on 16/8/25.
//

#include "Spectrum.h"

int Spectrum::BAD_QUALITY_MEASURE = -1;

Spectrum::Spectrum() {};

Spectrum::Spectrum(string &pId, int pPrecursorCharge, float pPrecursorMz, IQualityScorer* qualityScorer,
                    vector<Peak> &inpeaks) {
    this->id = pId;
    this->precursorCharge = pPrecursorCharge;
    this->precursorMz = pPrecursorMz;
    this->qualityScorer = qualityScorer;
    this->peaks.clear();
    for(const Peak& peak:inpeaks){
        peaks.push_back(peak);
    }
    sort(peaks.begin(),peaks.end(),Peak::cmpPeakMz);
    calculateIntensities();
}

Spectrum::Spectrum(  ISpectrum& spectrum){
    vector<Peak> peaks = spectrum.getPeaks();
    new (this)Spectrum(spectrum,peaks);
}

Spectrum::Spectrum(  ISpectrum& spectrum,   vector<Peak>& inpeaks){
    new (this)Spectrum(spectrum,inpeaks,false);
}

Spectrum::Spectrum(  ISpectrum& spectrum,   vector<Peak>& inpeaks, bool isSortedList) {
    this->id = spectrum.getId();
    this->precursorCharge = spectrum.getPrecursorCharge();
    this->precursorMz = spectrum.getPrecursorMz();
    this->qualityScorer = spectrum.getQualityScorer();

    peaks.clear();
    peaks = inpeaks;

    if(!isSortedList)
        sort(peaks.begin(),peaks.end(),Peak::cmpPeakMz);

    Properties props = spectrum.getProperties();
    properties.putAll(props);
    calculateIntensities();
}

void Spectrum::calculateIntensities() {
    double totalIntensityX = 0;
    double sumSquareIntensityX = 0;
    for(Peak& peak:peaks){
        double intensity = peak.getIntensity();
        totalIntensityX += intensity;
        double ji = convertIntensity(peak);
        sumSquareIntensityX += ji *ji;
    }
    totalIntensity = totalIntensityX;
    sumSquareIntensity = sumSquareIntensityX;
}

double Spectrum::convertIntensity(Peak &p1) {
    double intensity = p1.getIntensity();
    if(intensity == 0) return 0;
    return 1 + log(intensity) ;
}


vector<int> Spectrum::asMajorPeakMZs(int majorPeakCount) {

}

ISpectrum* Spectrum::asMajorPeaks(int majorPeakCount) {
    return getHighestNPeaks(majorPeakCount);
}

void Spectrum::guaranteeMajorPeaks(int majorPeakCount) {
    if (majorPeakMZ.size() != majorPeakCount) {
        majorPeakMZ.clear();
        ISpectrum* peaks = asMajorPeaks(majorPeakCount);
        for (Peak &peak : peaks->getPeaks()) {
            majorPeakMZ.push_back((int) peak.getMz());
        }
    }
}

ISpectrum* Spectrum::getHighestNPeaks(int numberRequested) {
//    Spectra in highestPeaks live with this class;
    ISpectrum *ret;
    try {
        ret = highestPeaks.at(numberRequested);
    }catch (out_of_range) {
        ISpectrum *replace = new Spectrum(buildHighestPeaks(numberRequested));
        ret = replace;
        int numberPeaks = ret->getPeaksCount();
        for (int i = numberPeaks; i >= numberPeaks; i--) {
            const int num = i;
            highestPeaks.insert(pair<int, ISpectrum*>(num, ret));
        }
    }
    return ret;

}

Spectrum Spectrum::buildHighestPeaks(int numberRequested)  {
    vector<Peak> byIntensity = this->getPeaks();
    sort(byIntensity.begin(),byIntensity.end(),Peak::cmpPeakIntensity);
    vector<Peak> holder;
    vector<Peak>::iterator iter;
    for(iter = byIntensity.begin();iter != byIntensity.end();++iter){
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

vector<Peak> Spectrum::getPeaks() {
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
bool Spectrum::cmpSpectrumMZ(ISpectrum* A,ISpectrum *B){
    float A_MZ = A->getPrecursorMz();
    float B_MZ = B->getPrecursorMz();
    return (IOUtilities::compare(A_MZ,B_MZ) == -1) ;
}

bool Spectrum::operator==( ISpectrum& O) const{
    if (precursorCharge != O.getPrecursorCharge()) return false;
    if (IOUtilities::compare(O.getPrecursorMz(), precursorMz) != 0) return false;
    if ( id != (O.getId())) return false;
    if (peaks.size() != (O.getPeaks().size())) {
        return false;
    }
    vector<Peak> peaks1 = peaks;
    vector<Peak>::iterator iter1;
    vector<Peak>::iterator iter2;
    iter1= peaks1.begin();
    iter2 = O.getPeaks().begin();

    for(iter1,iter2;iter1 != peaks1.end();++iter1,++iter2){
        if ( ! (*iter1 == *iter2)) {
            return false;
        }
    }

    return true;
}

Spectrum& Spectrum::operator=( Spectrum& O) {
    this->properties = O.getProperties();
    this->currentMAjorPeakCount = O.getPeaksCount();
    this->totalIntensity = O.getTotalIntensity();
    this->peaks = O.getPeaks();
    this->precursorCharge = O.getPrecursorCharge();
    this->precursorMz = O.getPrecursorMz();
    this->sumSquareIntensity = O.getSumSquareIntensity();
    this->id = O.getId();
}
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
    unordered_map<int,ISpectrum*>::iterator iter;
    for(iter = highestPeaks.begin();iter != highestPeaks.end();++iter){
        delete (*iter).second;
    }
}



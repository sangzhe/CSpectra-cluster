//
// Created by SangZhe on 16/8/30.
//

#include "Peak.h"
#include "../util/IOUtilities.h"


Peak::Peak(const float& massChargeRatio, const float& intensity) {
    this->massChargeRatio = massChargeRatio;
    this->intensity = intensity;
}

Peak::Peak(const float& massChargeRatio, const float& intensity, int count) {
    this->massChargeRatio = massChargeRatio;
    this->intensity = intensity;
    this->count = count;
}

Peak::Peak(const IPeak& copied) {
    this->massChargeRatio = copied.getMz();
    this->intensity = copied.getIntensity();
    this->count = copied.getCount();
}


int Peak::getCount()const  {
    return count;
}

float Peak::getIntensity() const {
    return intensity;
}

float Peak::getMz() const {
    return massChargeRatio;
}

int Peak::cmpPeak(IPeak* A,IPeak* B) {
    float Amz = A->getMz();
    float Bmz = B->getMz();

    int ret = IOUtilities::compare(Amz,Bmz);
    if(ret != 0) return ret;

    float Aintent = A->getIntensity();
    float Bintent = B->getIntensity();

    ret = IOUtilities::compare(Aintent,Bintent);
    if(ret != 0) return ret;

    return 0;
}

int Peak::cmpPeakMz(IPeak *A, IPeak *B) {

    return IOUtilities::compare(A->getMz(),B->getMz());
}

int Peak::cmpPeakIntensity(IPeak* A, IPeak* B) {
    if (A == nullptr){
        return (B == nullptr? 0:-1);
    }
    if(B == nullptr){
        return 1;
    }
    if(A->getIntensity() != B->getIntensity()){
        return (B->getIntensity() < A->getIntensity()? -1:1);
    }
    return 0;
}

string Peak::toString() {
    string mz = IOUtilities::FloatToString(this->getMz(),"%10.5f");
    string intensity = IOUtilities::FloatToString(this->getIntensity(),"%8.2");
    string mzTitle = "m/z = ";
    string intensTitle = ", intensity = ";
    string countTitle = ", count = ";
    string count = IOUtilities::IntToString(this->getCount(),"");
    string ret = mzTitle + mz + intensTitle + intensity + countTitle + count;
    return ret;
}

bool Peak::operator==(const IPeak &O){


    if (IOUtilities::compare(O.getIntensity(), intensity) != 0) return false;
    return IOUtilities::compare(O.getMz(), massChargeRatio) == 0;
}

//bool Peak::operator<(const IPeak &O) const {
//    return (IOUtilities::compare(intensity,O.getIntensity()) == -1);
//}

//Peak& Peak::operator=(const IPeak &O){
//    this->count = O.getCount();
//    this->intensity = O.getIntensity();
//    this->massChargeRatio = O.getMz();
//}

//size_t hash_value(const Peak &p) {
//    size_t seed = 0;
//    boost::hash_combine(seed,p.intensity);
//    boost::hash_combine(seed,p.massChargeRatio);
//    boost::hash_combine(seed,p.count);
//    return seed;
//}

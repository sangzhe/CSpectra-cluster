//
// Created by SangZhe on 16/8/30.
//

#include "Peak.h"
#include "../util/IOUtilities.h"

Peak Peak::null = Peak();

Peak::Peak(const float& massChargeRatio, const float& intensity) {
    new(this)Peak(massChargeRatio,intensity,1);
}

Peak::Peak(const float& massChargeRatio, const float& intensity, int count) {
    this->massChargeRatio = massChargeRatio;
    this->intensity = intensity;
    this->count = count;
}

Peak::Peak(const Peak& copied) {
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

bool Peak::cmpPeak(const Peak& A,const Peak& B) {
    float Amz = A.getMz();
    float Bmz = B.getMz();

    int ret = IOUtilities::compare(Amz,Bmz);
    if(ret != 0) return (ret != 1);

    float Aintent = A.getIntensity();
    float Bintent = B.getIntensity();

    ret = IOUtilities::compare(Aintent,Bintent);
    if(ret != 0) (ret != 1);

    return 0;
}

bool Peak::cmpPeakMz(const Peak&A, const Peak&B) {

    int ret = IOUtilities::compare(A.getMz(),B.getMz());

    return ( ret == -1);
}

bool Peak::cmpPeakIntensity(const Peak& A, const Peak& B) {
    Peak a = A;
    Peak b = B;
    if (a == Peak::null){
        return (b == Peak::null);
    }
    if(b == Peak::null){
        return 1;
    }
    if(a.getIntensity() != b.getIntensity()){
        return (b.getIntensity() < a.getIntensity());
    }
    return 0;
}

string Peak::toString() {
    string mz = IOUtilities::FloatToString(this->getMz(),"%10.5f");
    string intensity = IOUtilities::FloatToString(this->getIntensity(),"%8.2f");
    string mzTitle = "m/z = ";
    string intensTitle = ", intensity = ";
    string countTitle = ", count = ";
    string count = IOUtilities::IntToString(this->getCount(),"%d");
    string ret = mzTitle + mz + intensTitle + intensity + countTitle + count;
    return ret;
}

bool Peak::operator==(const Peak &O) const{
    if (IOUtilities::compare(O.getIntensity(), intensity) != 0) return false;
    return (IOUtilities::compare(O.getMz(), massChargeRatio) == 0);
}

bool Peak::operator<(const Peak &O) const {
    return (IOUtilities::compare(intensity,O.getIntensity()) == -1);
}

Peak& Peak::operator=(const Peak &O){
    this->count = O.getCount();
    this->intensity = O.getIntensity();
    this->massChargeRatio = O.getMz();
}

size_t hash_value(const Peak &p) {
    size_t seed = 0;
    boost::hash_combine(seed,p.intensity);
    boost::hash_combine(seed,p.massChargeRatio);
    boost::hash_combine(seed,p.count);
    return seed;
}

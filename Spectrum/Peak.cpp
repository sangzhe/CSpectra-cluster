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

Peak::Peak(IPeak& copied) {
    this->massChargeRatio = copied.getMz();
    this->intensity = copied.getIntensity();
    this->count = copied.getCount();
}

int Peak::getCount() {
    return count;
}

float Peak::getIntensity() {
    return intensity;
}

float Peak::getMz() {
    return massChargeRatio;
}

bool Peak::cmpPeak(IPeak* A,IPeak* B) {
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

bool Peak::cmpPeakMz(IPeak *A, IPeak *B) {

    return IOUtilities::compare(A->getMz(),B->getMz());
}

bool Peak::cmpPeakIntensity(IPeak *A, IPeak *B) {
    if (A == NULL){
        return (B == NULL? 0:-1);
    }
    if(B == NULL){
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

bool Peak::operator==( IPeak &O) {

    if (IOUtilities::compare(O.getIntensity(), intensity) != 0) return false;
    return IOUtilities::compare(O.getMz(), massChargeRatio) == 0;
}

Peak::~Peak(){}
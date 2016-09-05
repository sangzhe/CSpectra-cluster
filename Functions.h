//
// Created by SangZhe on 16/8/30.
//

#ifndef CSPECTRA_CLUSTER_FUNCTIONS_H
#define CSPECTRA_CLUSTER_FUNCTIONS_H

#include "vector"
#include "string"
#include "ISpectrum.h"
#include "math.h"
#include "stdlib.h"

using namespace std;
class Functions {
public:
    static void replace(string &string1,const string& Old,const string& New);
    static void split(const string& src, const string& separator, vector<string>& dest);

    static int compare(const float A,const float B);

    static string FloatToString(const float& Num,const string& accuracy);

    static string IntToString(const int& Num,const string& accuracy);

    void RemoveImpossiblyHighPeaksFunction(ISpectrum* spectrum);

    void RemovePrecursorPeaksFunction(ISpectrum* spectrum,float fragmentIonTolerance);

    void HighestNSpectrumPeaksFunction(ISpectrum* spectrum, int maxPeaks);
};


#endif //CSPECTRA_CLUSTER_FUNCTIONS_H

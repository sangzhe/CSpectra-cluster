//
// Created by SangZhe on 16/9/7.
//

#ifndef CSPECTRA_CLUSTER_MZINTENSITYUTILITIES_H
#define CSPECTRA_CLUSTER_MZINTENSITYUTILITIES_H


class MZIntensityUtilities {
public:
    static  double SMALL_MZ_DIFFERENCE ;

    static  double SMALL_INTENSITY_DIFFERENCE ;

    static  int HIGHEST_USABLE_MZ ; // ignore peaks higher than this

    static  int LOWEST_USABLE_MZ; // ignore peaks lower than this

    static  int MZ_RESOLUTION ; // we care about differences of 0.01 dalton

//  Rounding factor to use. 1000 means 3 positions after the comma.
    static int MZ_PRECISION;

    static int mzToInt(double mz);

    static double round(double f, int decimalPlace);

};


#endif //CSPECTRA_CLUSTER_MZINTENSITYUTILITIES_H

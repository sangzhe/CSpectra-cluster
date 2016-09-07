//
// Created by SangZhe on 16/9/7.
//

#include "MZIntensityUtilities.h"

double MZIntensityUtilities::SMALL_MZ_DIFFERENCE = 0.002;

double MZIntensityUtilities::SMALL_INTENSITY_DIFFERENCE = 0.1;

int MZIntensityUtilities::HIGHEST_USABLE_MZ = 5000;

int MZIntensityUtilities::LOWEST_USABLE_MZ = 50;

int MZIntensityUtilities::MZ_RESOLUTION = 1000;

int MZIntensityUtilities::MZ_PRECISION = 1000;

int MZIntensityUtilities::mzToInt(double mz) {
    return (int)((MZ_RESOLUTION * mz) + 0.5);
}

double MZIntensityUtilities::round(double f, int decimalPlace) {
    int i = (int) ((f * decimalPlace) + 0.5);
    return i / (double) decimalPlace;
}
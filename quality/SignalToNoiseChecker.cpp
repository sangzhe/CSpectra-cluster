//
// Created by SangZhe on 16/9/1.
//

#include "SignalToNoiseChecker.h"

string SignalToNoiseChecker::VERSION = "1.0";

int SignalToNoiseChecker::NUMBER_HIGH_PEAKS = 8;

SignalToNoiseChecker::SignalToNoiseChecker() {};

string SignalToNoiseChecker::getCurrentVersion() {
    return VERSION;
}

double SignalToNoiseChecker::calculateQualityScore(Spectrum& spectrum) {
    Spectrum highestNPeaks(spectrum.getHighestNPeaks(NUMBER_HIGH_PEAKS));
    if(highestNPeaks.getPeaksCount() < NUMBER_HIGH_PEAKS) return 0.0;

    double totalIntensity = highestNPeaks.getTotalIntensity();
    double highestPeak = 0;
    list<Peak> highestPeaks = highestNPeaks.getPeaks();
    list<Peak>::iterator iter;
    for(iter=highestPeaks.begin();iter != highestPeaks.end();iter++){
        Peak peak = *iter;
        highestPeak = std::max((double)peak.getIntensity(),highestPeak);
    }


    double meanHigh = (totalIntensity - highestPeak) / (NUMBER_HIGH_PEAKS -1);
    list<Peak> peaks = spectrum.getPeaks();
    peaks.sort(Peak::cmpPeakIntensity);

    double median;

    int peakSize = peaks.size();
    if(peakSize % 2 == 1){
        int index =peakSize / 2 ;
        list<Peak>::iterator iter;
        Peak peak;
        int i =0;
        for(iter = peaks.begin();iter != peaks.end(); iter++){
            if ( i == index ) {
                peak = *iter;
                break;
            }
            else i++;
        }
        median = peak.getIntensity();
    }
    else{
        int index2 = (peakSize / 2 );
        int index1 = index2 - 1 ;
        list<Peak>::iterator iter;
        Peak peak1;
        Peak peak2;
        int i = 0;
        for(iter = peaks.begin();iter != peaks.end();iter++){
            if( i == index1 ){
                peak1 = *iter;
            }
            else if (i == index2 ){
                peak2 = *iter;
                break;
            }
            else i++;
        }
        double intensity1 = peak1.getIntensity();
        double intensity2 =  peak2.getIntensity();
        median = (intensity1 + intensity2) / 2 ;
    }
    return meanHigh / median ;
}

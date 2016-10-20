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

double SignalToNoiseChecker::calculateQualityScore( ISpectrum& spectrum) {

    ISpectrum *highestNPeaks = spectrum.getHighestNPeaks(NUMBER_HIGH_PEAKS);
    if(highestNPeaks->getPeaksCount() < NUMBER_HIGH_PEAKS) return 0.0;

    double totalIntensity = highestNPeaks->getTotalIntensity();
    double highestPeak = 0;
    vector<Peak> highestPeaks = highestNPeaks->getPeaks();
    vector<Peak>::iterator iter;
    for(iter=highestPeaks.begin();iter != highestPeaks.end();++iter){
        Peak peak = *iter;
        highestPeak = max((double)peak.getIntensity(),highestPeak);
    }


    double meanHigh = (totalIntensity - highestPeak) / (NUMBER_HIGH_PEAKS -1);
    vector<Peak> peaks = spectrum.getPeaks();
    sort(peaks.begin(),peaks.end(),Peak::cmpPeakIntensity);

    double median;

    size_t peakSize = peaks.size();
    if(peakSize % 2 == 1){
        size_t index =peakSize / 2 ;
        Peak peak;
        int i =0;
        for(Peak& p:peaks){
            if ( i == index ) {
                peak = p;
                break;
            }
            else i++;
        }
        median = peak.getIntensity();
    }
    else{
        size_t index2 = (peakSize / 2 );
        size_t index1 = index2 - 1 ;
        Peak peak1;
        Peak peak2;
        int i = 0;
        for(Peak& p:peaks){
            if( i == index1 ){
                peak1 = p;
            }
            else if (i == index2 ){
                peak2 = p;
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

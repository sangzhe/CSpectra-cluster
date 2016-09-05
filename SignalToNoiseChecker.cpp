//
// Created by SangZhe on 16/9/1.
//

#include "SignalToNoiseChecker.h"

static const string VERSION = "1.0";

static const int NUMBER_HIGH_PEAKS = 8;

string SignalToNoiseChecker::getCurrentVersion() {
    return VERSION;
}

double SignalToNoiseChecker::calculateQualityScore(ISpectrum& spectrum) {
    ISpectrum* highestNPeaks = spectrum.getHighestNPeaks(NUMBER_HIGH_PEAKS);
    if(highestNPeaks->getPeaksCount() < NUMBER_HIGH_PEAKS) return 0.0;

    double totalIntensity = highestNPeaks->getTotalIntnesity();
    double highestPeak = 0;
    list<IPeak*> highestPeaks = highestNPeaks->getPeaks();
    list<IPeak*>::iterator iter;
    for(iter=highestPeaks.begin();iter != highestPeaks.end();iter++){
        IPeak* peak = *iter;
        highestPeak = std::max((double)peak->getIntensity(),highestPeak);
    }


    double meanHigh = (totalIntensity - highestPeak) / (NUMBER_HIGH_PEAKS -1);
    list<IPeak*> peaks = spectrum.getPeaks();
    peaks.sort(Peak::cmpPeakIntensity);

    double median;

    int peakSize = peaks.size();
    if(peakSize % 2 == 1){
        int index =peakSize / 2 ;
        list<IPeak*>::iterator iter;
        IPeak* iPeak;
        int i =0;
        for(iter = peaks.begin();iter != peaks.end(); iter++){
            if ( i == index ) {
                iPeak = *iter;
                break;
            }
            else i++;
        }
        median = iPeak->getIntensity();
    }
    else{
        int index2 = (peakSize / 2 );
        int index1 = index2 - 1 ;
        list<IPeak*>::iterator iter;
        IPeak* iPeak1;
        IPeak* iPeak2;
        int i = 0;
        for(iter = peaks.begin();iter != peaks.end();iter++){
            if( i == index1 ){
                iPeak1 = *iter;
            }
            else if (i == index2 ){
                iPeak2 = *iter;
                break;
            }
            else i++;
        }
        double intensity1 = iPeak1->getIntensity();
        double intensity2 =  iPeak2->getIntensity();
        median = (intensity1 + intensity2) / 2 ;
        delete iPeak1,iPeak2;
        iPeak1,iPeak2 = nullptr;
    }
    delete highestNPeaks;
    return meanHigh / median ;
}
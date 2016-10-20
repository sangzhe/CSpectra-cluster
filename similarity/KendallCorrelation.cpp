//
// Created by SangZhe on 2016/9/26.
//

#include "KendallCorrelation.h"

long KendallCorrelation::sum(long n) {
    return n * (n + 1L) / 2L;
}

bool KendallCorrelation::compare(pair<double, double> pair1, pair<double, double> pair2) {
//    int compareFirst = ((Double)pair1.getFirst()).compareTo((Double)pair2.getFirst());
    int compareFirst = IOUtilities::compare(pair1.first,pair2.first);
    if(compareFirst != 0) return (compareFirst == -1);

    int compareSecond = IOUtilities::compare(pair1.second,pair2.second);
    if(compareSecond != 0) return (compareSecond == -1);

    return false;
}

double KendallCorrelation::correlation(vector<double> xArray, vector<double> yArray) {
    if (xArray.size() != yArray.size()) {
        throw(xArray.size(), yArray.size());
    }

     int n = xArray.size();
     long numPairs = sum(n - 1);

    vector<pair<double,double>> pairs(n);
    for (int i = 0; i < n; i++) {
        pairs[i] = pair<double, double>(xArray[i], yArray[i]);
    }

    sort(pairs.begin(),pairs.end(),compare);


    long tiedXPairs = 0;
    long tiedXYPairs = 0;
    long consecutiveXTies = 1;
    long consecutiveXYTies = 1;
    pair<double, double> prev = pairs[0];
    for (int i = 1; i < n; i++) {
         pair<double, double> curr = pairs[i];
        if (curr.first == prev.first) {
            consecutiveXTies++;
            if (curr.second == prev.second) {
                consecutiveXYTies++;
            } else {
                tiedXYPairs += sum(consecutiveXYTies - 1);
                consecutiveXYTies = 1;
            }
        } else {
            tiedXPairs += sum(consecutiveXTies - 1);
            consecutiveXTies = 1;
            tiedXYPairs += sum(consecutiveXYTies - 1);
            consecutiveXYTies = 1;
        }
        prev = curr;
    }
    tiedXPairs += sum(consecutiveXTies - 1);
    tiedXYPairs += sum(consecutiveXYTies - 1);

    long swaps = 0;
    vector<pair<double, double>> pairsDestination(n);
    for (int segmentSize = 1; segmentSize < n; segmentSize <<= 1) {
        for (int offset = 0; offset < n; offset += 2 * segmentSize) {
            int i = offset;
             int iEnd = min(i + segmentSize, n);
             int j = iEnd;
             int jEnd = min(j + segmentSize, n);

            int copyLocation = offset;
            while (i < iEnd || j < jEnd) {
                if (i < iEnd) {
                    if (j < jEnd) {
                        if (IOUtilities::compare(pairs[i].second,pairs[j].second) <= 0) {
                            pairsDestination[copyLocation] = pairs[i];
                            i++;
                        } else {
                            pairsDestination[copyLocation] = pairs[j];
                            j++;
                            swaps += iEnd - i;
                        }
                    } else {
                        pairsDestination[copyLocation] = pairs[i];
                        i++;
                    }
                } else {
                    pairsDestination[copyLocation] = pairs[j];
                    j++;
                }
                copyLocation++;
            }
        }
        vector<pair<double, double>> pairsTemp = pairs;
        pairs = pairsDestination;
        pairsDestination = pairsTemp;
    }

    long tiedYPairs = 0;
    long consecutiveYTies = 1;
    prev = pairs[0];
    for (int i = 1; i < n; i++) {
         pair<double, double> curr = pairs[i];
        if (curr.second == prev.second) {
            consecutiveYTies++;
        } else {
            tiedYPairs += sum(consecutiveYTies - 1);
            consecutiveYTies = 1;
        }
        prev = curr;
    }
    tiedYPairs += sum(consecutiveYTies - 1);

     long concordantMinusDiscordant = numPairs - tiedXPairs - tiedYPairs + tiedXYPairs - 2 * swaps;
     double nonTiedPairsMultiplied = (numPairs - tiedXPairs) * (double) (numPairs - tiedYPairs);
    return concordantMinusDiscordant /sqrt(nonTiedPairsMultiplied);
}

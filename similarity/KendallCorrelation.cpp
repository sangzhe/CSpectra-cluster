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
    if(xArray.size() != yArray.size()) {
        throw(xArray.size(), yArray.size());
    } else {
        int n = xArray.size();
        long numPairs = sum((long)(n - 1));
        vector<pair<double,double>> pairs(n);

        for(int tiedXPairs = 0; tiedXPairs < n; ++tiedXPairs) {
            pairs[tiedXPairs] = pair<double,double>(xArray[tiedXPairs], yArray[tiedXPairs]);
        }


        sort(pairs.begin(),pairs.end(),compare);

        long var26 = 0L;
        long tiedXYPairs = 0L;
        long consecutiveXTies = 1L;
        long consecutiveXYTies = 1L;
        pair<double,double> prev = pairs[0];

        int swaps;
        for(swaps = 1; swaps < n; ++swaps) {
            pair<double,double> pairsDestination = pairs[swaps];
            if((pairsDestination.first) == (prev.first)) {
                ++consecutiveXTies;
                if((pairsDestination.second) == (prev.second)) {
                    ++consecutiveXYTies;
                } else {
                    tiedXYPairs += sum(consecutiveXYTies - 1L);
                    consecutiveXYTies = 1L;
                }
            } else {
                var26 += sum(consecutiveXTies - 1L);
                consecutiveXTies = 1L;
                tiedXYPairs += sum(consecutiveXYTies - 1L);
                consecutiveXYTies = 1L;
            }

            prev = pairsDestination;
        }

        var26 += sum(consecutiveXTies - 1L);
        tiedXYPairs += sum(consecutiveXYTies - 1L);
        swaps = 0;
        vector<pair<double,double>> var27(n);

        int concordantMinusDiscordant;
        for(int tiedYPairs = 1; tiedYPairs < n; tiedYPairs <<= 1) {
            for(int pairsTemp = 0; pairsTemp < n; pairsTemp += 2 * tiedYPairs) {
                int consecutiveYTies = pairsTemp;
                int iEnd = min(pairsTemp + tiedYPairs, n);
                concordantMinusDiscordant = iEnd;
                int curr = min(iEnd + tiedYPairs, n);

                for(int nonTiedPairsMultiplied = pairsTemp; consecutiveYTies < iEnd || concordantMinusDiscordant < curr; ++nonTiedPairsMultiplied) {
                    if(consecutiveYTies < iEnd) {
                        if(concordantMinusDiscordant < curr) {
                            if((pairs[consecutiveYTies].second) == (pairs[concordantMinusDiscordant].second) <= 0) {
                                var27[nonTiedPairsMultiplied] = pairs[consecutiveYTies];
                                ++consecutiveYTies;
                            } else {
                                var27[nonTiedPairsMultiplied] = pairs[concordantMinusDiscordant];
                                ++concordantMinusDiscordant;
                                swaps += iEnd - consecutiveYTies;
                            }
                        } else {
                            var27[nonTiedPairsMultiplied] = pairs[consecutiveYTies];
                            ++consecutiveYTies;
                        }
                    } else {
                        var27[nonTiedPairsMultiplied] = pairs[concordantMinusDiscordant];
                        ++concordantMinusDiscordant;
                    }
                }
            }

            vector<pair<double,double>> var29 = pairs;
            pairs = var27;
            var27 = var29;
        }

        long var28 = 0L;
        long var30 = 1L;
        prev = pairs[0];

        for(concordantMinusDiscordant = 1; concordantMinusDiscordant < n; ++concordantMinusDiscordant) {
            pair<double,double> var31 = pairs[concordantMinusDiscordant];
            if((var31.second) == (prev.second)) {
                ++var30;
            } else {
                var28 += sum(var30 - 1L);
                var30 = 1L;
            }

            prev = var31;
        }

        var28 += sum(var30 - 1L);
        long var32 = numPairs - var26 - var28 + tiedXYPairs - (long)(2 * swaps);
        double var33 = (double)(numPairs - var26) * (double)(numPairs - var28);
        return (double)var32 / sqrt(var33);
    }
}
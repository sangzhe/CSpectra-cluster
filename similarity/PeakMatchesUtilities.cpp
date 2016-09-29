//
// Created by SangZhe on 2016/9/22.
//

#include "PeakMatchesUtilities.h"
PointerPool* PeakMatchesUtilities::pointer_pool = PoolFactory::getInstance();


IPeak *PeakMatchesUtilities::LAST_PEAK = new Peak(IOUtilities::FLOAT_MAX_VALUE, 0);

IPeakMatches* PeakMatchesUtilities::getSharedPeaksAsMatches(ISpectrum *spectrum1, ISpectrum *spectrum2,
                                                            float mzTolerance, bool applyNPeaksFilter) {
    // get similar peaks
    ISpectrum *filteredSpectrum1, *filteredSpectrum2;

    if (applyNPeaksFilter) {
        int nPeaks = calculateNPeaks(spectrum1->getPrecursorMz(), spectrum2->getPrecursorMz());
        if (nPeaks < 20)
            nPeaks = 20;

        filteredSpectrum1 = spectrum1->getHighestNPeaks(nPeaks);
        filteredSpectrum2 = spectrum2->getHighestNPeaks(nPeaks);

    } else {
        // simply disable filtering
        filteredSpectrum1 = spectrum1;
        pointer_pool->add(filteredSpectrum1);
        filteredSpectrum2 = spectrum2;
        pointer_pool->add(filteredSpectrum2);

    }

    IPeakMatches *ret = PeakMatchesUtilities::getSharedPeaksAsMatches(filteredSpectrum1, filteredSpectrum2, mzTolerance);
    pointer_pool->remove(filteredSpectrum1);
    pointer_pool->remove(filteredSpectrum2);
    return ret;
}

IPeakMatches* PeakMatchesUtilities::getSharedPeaksAsMatches(ISpectrum *spectrum1, ISpectrum *spectrum2,
                                                           float mzTolerance) {
    vector<vector<int>> sharedPeakIndices = getSharedPeaks(*spectrum1, *spectrum2, mzTolerance);
    IPeakMatches *ret = new PeakMatches(spectrum1, spectrum2, sharedPeakIndices[0], sharedPeakIndices[1]);
    pointer_pool->add(ret);
    return ret;
}

vector<vector<int>> PeakMatchesUtilities::getSharedPeaks(const ISpectrum &spectrum1, const ISpectrum &spectrum2,
                                                         float mzTolerance) {
    vector<IPeak*> peaks1 = spectrum1.getPeaks();
    vector<IPeak*> peaks2 = spectrum2.getPeaks();

    vector<int> sharedPeaksIndexes1 ;
    vector<int> sharedPeaksIndexes2 ;

    // upper and lower bound
    int indexSpec1 = 0, indexSpec2 = 0;

    while (indexSpec1 < peaks1.size() && indexSpec2 < peaks2.size()) {
        float mz1 = peaks1[indexSpec1]->getMz();
        float mz2 = peaks2[indexSpec2]->getMz();
        float difference = fabs(mz1 - mz2);

        if (difference > mzTolerance) {
            if (mz1 < mz2) {
                indexSpec1++;
            } else {
                indexSpec2++;
            }
        }
            // a potential match was found
        else {

            float differenceNextSpec1Spec2 = (indexSpec1 < peaks1.size() - 1 && indexSpec2 < peaks2.size() - 1) ?
                                             fabs(peaks1[indexSpec1 + 1]->getMz() - peaks2[indexSpec2 + 1]->getMz()) :
                                             IOUtilities::FLOAT_MAX_VALUE;

            // if the next two peaks are also a match, just match the current two
            if (differenceNextSpec1Spec2 > mzTolerance) {
                float differenceNextSpec1 = (indexSpec1 < peaks1.size() - 1) ?
                                            fabs(peaks1[indexSpec1 + 1]->getMz() - mz2) :
                                            IOUtilities::FLOAT_MAX_VALUE;

                float differenceNextSpec2 = (indexSpec2 < peaks2.size() - 1) ?
                                            fabs(peaks2[indexSpec2 + 1]->getMz() - mz1) :
                                            IOUtilities::FLOAT_MAX_VALUE;

                // using next peak in spec 1 is the best match
                if (differenceNextSpec1 < difference && differenceNextSpec1 < differenceNextSpec2) {
                    indexSpec1++;
                }
                    // using next peak in spec 2 is the best match
                else if (differenceNextSpec2 < difference) {
                    indexSpec2++;
                }
            }

            sharedPeaksIndexes1.push_back(indexSpec1);
            sharedPeaksIndexes2.push_back(indexSpec2);

            indexSpec1++;
            indexSpec2++;
        }
    }

    vector<vector<int>> result = vector<vector<int>>(2);
    result[0] = sharedPeaksIndexes1;
    result[1] = sharedPeaksIndexes2;

    return result;
}

vector<vector<int>> PeakMatchesUtilities::getSharedPeaks2(const ISpectrum &spectrum1, const ISpectrum &spectrum2,
                                                          float mzTolerance) {
    vector<IPeak*> peaks1 = spectrum1.getPeaks();
    vector<IPeak*> peaks2= spectrum2.getPeaks();

    peaks1.push_back(LAST_PEAK);
    peaks2.push_back(LAST_PEAK);

    vector<int> sharedPeaksSpec1 ;
    vector<int> sharedPeaksSpec2 ;

    bool spec1IsLast = false;
    int indexSpec1 = 0;
    int indexSpec2 = 0;

    while (indexSpec1 < peaks1.size() - 1 && indexSpec2 < peaks2.size() - 1) {
        IPeak* peak1 = peaks1[indexSpec1];
        IPeak* peak2 = peaks2[indexSpec2];

        double mz1 = peak1->getMz();
        double mz2 = peak2->getMz();
        double mzDifference = fabs(mz1 - mz2);

        if (fabs(mzDifference) <= mzTolerance) {
            // also calculate the difference for the next t and e peaks
            IPeak *nextPeak1 = peaks1[indexSpec1 + 1];
            IPeak *nextPeak2 = peaks2[indexSpec2 + 1];
            double mzDifferenceNextSpec1 = fabs(mz2 - nextPeak1->getMz());
            double mzDifferenceNextSpec2 = fabs(nextPeak2->getMz() - mz1);
            double mzDifferenceNextSpec12 = fabs(nextPeak2->getMz() - nextPeak1->getMz());

            // use the next peak in spectrum 1 if it's a better match
            if (mzDifferenceNextSpec1 < mzDifference &&
                mzDifferenceNextSpec1 < mzDifferenceNextSpec2 &&
                mzDifferenceNextSpec1 < mzDifferenceNextSpec12) {
                indexSpec1++;
            }

            // use the next peak in spectrum 2 if it's a better match
            if (mzDifferenceNextSpec2 < mzDifference &&
                mzDifferenceNextSpec2 < mzDifferenceNextSpec1 &&
                mzDifferenceNextSpec2 < mzDifferenceNextSpec12) {
                indexSpec2++;
            }

            sharedPeaksSpec1.push_back(indexSpec1);
            sharedPeaksSpec2.push_back(indexSpec2);

            // increment both counters since both peaks must not be compared again
            indexSpec1++;
            indexSpec2++;
            continue;
        }

        if (mzDifference == 0) {
            if (spec1IsLast) {
                indexSpec2++;
                spec1IsLast = false;
            } else {
                indexSpec1++;
                spec1IsLast = true;
            }
        } else {
            if (mz1 < mz2) {
                indexSpec1++;
            } else {
                indexSpec2++;
            }
        }

    }

    vector<vector<int>> result = vector<vector<int>>(2);
    result[0] = sharedPeaksSpec1;
    result[1] = sharedPeaksSpec2;

    return result;
}


int PeakMatchesUtilities::calculateNPeaks(float precursor1, float precursor2) {
    // if any of the required values is missing, return 15
    if (precursor1 == 0 || precursor2 == 0)
        return 15;

    // use m/z / 50
    return (int) ((precursor1 / 50 + precursor2 / 50) / 2);
}
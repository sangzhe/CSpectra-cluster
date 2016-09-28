//
// Created by SangZhe on 2016/9/28.
//

#include "ShareMajorPeaksPredicate.h"
int ShareMajorPeaksPredicate::DEFAULT_MAJOR_PEAKS = 5;

ShareMajorPeaksPredicate::ShareMajorPeaksPredicate() {
    new(this)ShareMajorPeaksPredicate(DEFAULT_MAJOR_PEAKS);
}

ShareMajorPeaksPredicate::ShareMajorPeaksPredicate(int nMajorPeaks) {
    this->nMajorPeaks = nMajorPeaks;
}

bool ShareMajorPeaksPredicate::apply(ISpectrum *o1, ISpectrum *o2) {
    vector<int> majorPeaks1 = o1->asMajorPeakMZs(nMajorPeaks);
    vector<int> majorPeaks2 = o2->asMajorPeakMZs(nMajorPeaks);

    // major peaks are always sorted according to m/z
    for (int index1 = 0, index2 = 0; index1 < majorPeaks1.size() && index2 < majorPeaks2.size(); ) {
        if (majorPeaks1[index1] == majorPeaks2[index2])
            return true;

        if (majorPeaks1[index1] < majorPeaks2[index2]) {
            index1++;
        }
        else  if (majorPeaks1[index1] > majorPeaks2[index2]) {
            index2++;
        }
        else {
            // both are identical, increment the lower one
            if (index1 < index2)
                index1++;
            else
                index2++;
        }
    }

    return false;
}
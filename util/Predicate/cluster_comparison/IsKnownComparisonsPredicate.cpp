//
// Created by SangZhe on 2016/9/28.
//

#include "IsKnownComparisonsPredicate.h"
bool IsKnownComparisonsPredicate::apply(ICluster *o1, ICluster *o2) {
    // make sure both have comparison matches
    if (o1->getComparisonMatches().size() == 0 || o2->getComparisonMatches().size() == 0)
        return false;

    // check if they are known
    if (o1->isKnownComparisonMatch(o2->getId()) || o2->isKnownComparisonMatch(o1->getId()))
        return true;

    // not known
    return false;
}
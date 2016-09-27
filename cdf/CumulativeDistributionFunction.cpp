//
// Created by SangZhe on 2016/9/27.
//

#include "CumulativeDistributionFunction.h"

string CumulativeDistributionFunction::HEADER_LINE = "max_score\tlower_diff_matches\tcum_lower_diff_matches\trel_cum_lower_matches\ttotal_matches";

CumulativeDistributionFunction::CumulativeDistributionFunction(long totalComparisons, double scoreIncrements,
                                                               vector<double> proportionPeptidesBelowScore) {
    this->totalComparisons = totalComparisons;
    this->scoreIncrements = scoreIncrements;
    this->proportionPeptidesBelowScore = proportionPeptidesBelowScore;
}

CumulativeDistributionFunction CumulativeDistributionFunction::fromString(string String) throw(exception){
    if (String == "" || String.length() < 1)
        return CumulativeDistributionFunction();
    vector<string> items;
    IOUtilities::split(String,"\n",items);
    if (items.size() < 2)
        return CumulativeDistributionFunction();
    // sanity check whether the right format is used
    if (! (HEADER_LINE == (items[0])))
        throw("Unsupported format used. Header line does not match.");

    vector<string> firstFields;
    IOUtilities::split(items[1],"\t",firstFields);
    if (firstFields.size() != 5)
        throw("First line does not contain the expected 5 fields.");

    // save the total score increment - this is stable accross the whole file
    double scoreIncrement = IOUtilities::StringToDouble(firstFields[0]);
    long calculatedTotalComparisons = 0;
    // get the actual values
    vector<double> proportionPeptidesBelowScore;
    for (int i = 1; i < items.size(); i++) {
        string line = items[i];
        vector<string> fields;
        IOUtilities::split(line,"\t",fields);

        if (fields.size() != 5)
            throw("Line does not contain the expected 5 fields.");

        long numberCumulativePeptides = IOUtilities::StringToLong(fields[2]);
        long totalComparisons = IOUtilities::StringToLong(fields[4]); // total number can change at each step since this is an aggregated file from many single analysis

        double relCumulativePeptides = (double) numberCumulativePeptides / totalComparisons;

        calculatedTotalComparisons = max(calculatedTotalComparisons, totalComparisons);
        proportionPeptidesBelowScore.push_back(relCumulativePeptides);
    }

    return CumulativeDistributionFunction(calculatedTotalComparisons, scoreIncrement, proportionPeptidesBelowScore);


}
int CumulativeDistributionFunction::getBinForScore(double score) {
    double doubleBin = score / scoreIncrements;

    // use ceil to get the next higher scoring bin
    int index = (int) ceil(doubleBin);

    if (index >= proportionPeptidesBelowScore.size())
        index = proportionPeptidesBelowScore.size() - 1;

    if (index < 0) {
        index = 0;
    }

    return index;
}
double CumulativeDistributionFunction::getCdfForThreshold(double threshold) {
    int index = getBinForScore(threshold);

    return proportionPeptidesBelowScore[index];
}

double CumulativeDistributionFunction::probability(double threshold, int nComparisons) {
    double pValue = 1.0 - pow(getCdfForThreshold(threshold), nComparisons);

    return pValue;
}

bool CumulativeDistributionFunction::isSaveMatch(double similarity, int nComparisons,
                                                 double maximumMixtureProbability) {
    // get the estimated proportion of correct matches at this threshold / similarity
    double proportionCorrectMatches = pow(getCdfForThreshold(similarity), nComparisons);
    double minimumCorrectMatches = 1.0 - maximumMixtureProbability;

    return proportionCorrectMatches > minimumCorrectMatches;
}


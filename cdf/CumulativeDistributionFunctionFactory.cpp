//
// Created by SangZhe on 2016/9/27.
//

#include "CumulativeDistributionFunctionFactory.h"

CumulativeDistributionFunction CumulativeDistributionFunctionFactory::getCumulativeDistributionFunctionForSimilarityMetric(
        ISimilarityChecker *similarityCheckerClass)  throw(exception){
    if(similarityCheckerClass->getAlgorithmName() == "Combined FisherExact and Intensity rank test"){
        return getCumulativeDistributionFunctionForResource("cumulative.cdf.tsv");
    }
    if(similarityCheckerClass->getAlgorithmName() == "FrankEtAlDotProduct"){
        return getCumulativeDistributionFunctionForResource("dot.cdf.tsv");
    }

    throw("No cumulative distribution function defined for " + similarityCheckerClass->getAlgorithmName());
}

CumulativeDistributionFunction CumulativeDistributionFunctionFactory::getCumulativeDistributionFunctionForResource(
        string resource)  throw(exception){
    string filepath = "../recources/" + resource;
    ifstream fin(filepath.data());
    string ret = "";
    string line;
    while(getline(fin,line)){
        ret.append(line).append("\n");
    }
    return CumulativeDistributionFunction::fromString(ret);


}
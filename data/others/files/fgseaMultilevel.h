#pragma once 

#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::plugins(cpp17)]]
// [[Rcpp::export]]
NumericVector fgseaMultilevelCpp(const NumericVector& enrichmentScores, 
                                 const NumericVector& ranks, int pathwaySize,
                                 int sampleSize, int seed, 
                                 double absEps, bool sign);


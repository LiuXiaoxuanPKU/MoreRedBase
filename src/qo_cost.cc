#include "qo_cost.h"

float CostEstimator::CostNLJ(float Nouter, float Ninner, float scanOuterCost, float scanInnerCost) {
    return (CPU_OPERATOR_COST + CPU_TUPLE_COST) * Nouter * Ninner 
    + scanInnerCost * Nouter + scanOuterCost;
}

// INDEX SCAN COST is a parameter related to 
//                            the height of the index
//                            the number of tuples with the same key
// INDEX_SCAN_COST should not be a const
float CostEstimator::CostNLJIndex(float Nouter, float Ninner, float scanOuterCost, float scanInnerCost) {
    return (CPU_TUPLE_COST + INDEX_ACCESS_COST) * Nouter + scanOuterCost;
}
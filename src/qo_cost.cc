#include "qo_cost.h"
#include <iostream>

#define PG_ENCLAVE_COST

// ENCRYPT_COST, DECRYPT_COST are parameters related to data size
float CostEstimator::CostNLJ(float Nouter, float Ninner, float scanOuterCost, float scanInnerCost) {
    // std::cout << "Nouter : " << Nouter << " Ninner : " << Ninner;
    // std::cout << " scanInnerCost : " << scanInnerCost << " scanOuterCost : " << scanOuterCost << std::endl;
    #ifdef REDB_COST
    // Cost used by Redbase
    return Nouter * (1 + scanInnerCost);
    #endif

    // The inner table is kept in memory, so it's materialized
    // nested loop join
    float PGCost = 0;
    float rescanCost = CPU_OPERATOR_COST * Ninner;
    float materializeCost = scanInnerCost + 2 * CPU_OPERATOR_COST * Ninner;
    PGCost = (CPU_OPERATOR_COST + CPU_TUPLE_COST) * Nouter * Ninner 
                + rescanCost * (Nouter - 1) + scanOuterCost + materializeCost;

    #ifdef PG_COST
    // Cost used by Postgres 
    return PGCost;
    #endif

    #ifdef PG_ENCLAVE_COST
    // Cost used by Postgres + Enclave Cost
    return PGCost + (ECALL_COST + 2 * (ENCRYPT_COST + DECRYPT_COST)) * Nouter * Ninner;
    #endif
}

// INDEX_ACCESS_COST is a parameter related to 
//                            the height of the index
//                            the number of tuple buckets with the same key (can be approximated by Ninner / NinnerDistinct)
// INDEX_SCAN_COST should not be a const
float CostEstimator::CostNLJIndex(float Nouter, float Ninner,
                                  float scanOuterCost, float scanInnerCost,
                                  float NinnerDistinct) {
    #ifdef REDB_COST
    // Cost used by Redbase
    return Nouter * (Ninner / NinnerDistinct);
    #endif

    float PGCost = (CPU_TUPLE_COST + INDEX_ACCESS_COST) * Nouter + scanOuterCost;
    #ifdef PG_COST
    // Cost used by Postgres 
    return PGCost;
    #endif

    #ifdef PG_ENCLAVE_COST
    // Cost used by Postgres + Enclave Cost
    int selectedTupleNum = (Ninner / NinnerDistinct);
    return PGCost
            + selectedTupleNum * Nouter * (ECALL_COST + 2*(ENCRYPT_COST + DECRYPT_COST));
    #endif
}
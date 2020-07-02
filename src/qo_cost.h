#ifndef QO_COST_H
#define QO_COST_H


#define CPU_OPERATOR_COST 1
#define CPU_TUPLE_COST 1
#define INDEX_ACCESS_COST 1
#define ECALL_COST 1
#define OCALL_COST 1
#define ENCRYPT_COST 1
#define DECRYPT_COST 1



class CostEstimator {
public:
    CostEstimator();
    static float CostNLJ(float Nouter, float Ninner, float scanInnerCost, float scanOuterCost);
    static float CostNLJIndex(float Nouter, float Ninner, float scanOuterCost, float scanInnerCost, float NinnerDistinct);
};

#endif
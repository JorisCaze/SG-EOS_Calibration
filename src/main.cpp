#include <iostream>

#include "functions.h"

using namespace std;

int main()
{
    double T0,hL0,hG0,T1,hL1,hG1;
    double T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG;
    double cpL, qL, cpG, qG, pInfL, cvL, pInfG, cvG;

    // Read reference states
    readRefStates(T0,hL0,hG0,T1,hL1,hG1,T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG);

    // Compute Cp_k and q_k
    cpL = computeCpk(hL1,hL0,T1,T0);  // Liquid
    qL = computeQk(hL0,T0,cpL);
    
    cpG = computeCpk(hG1,hG0,T1,T0);  // Gas
    qG = computeQk(hG0,T0,cpG);

    // Compute pInf_k and gamma_k
    pInfL = computePinfK(vL0,vL1,T0,T1,pSat0,pSat1);
    cvL = computeCvk(cpL,vL0,T0,pSat0,pInfL);

    pInfG = computePinfK(vG0,vG1,T0,T1,pSat0,pSat1);
    cvG = computeCvk(cpG,vG0,T0,pSat0,pInfG);

    // Compute qPrim_k


    return 0;
}

#include <iostream>

#include "functionsRef.h"

using namespace std;

int main()
{
    double T0,hL0,hG0,T1,hL1,hG1;
    double T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG;
    double cpL, qL, cpG, qG, pInfL, cvL, gammaL, pInfG, cvG, gammaG;

    // *** Shock calibration ***
    

    // *** Liquid and its vapor calibration ***
    // Read reference states
    readRefStates(T0,hL0,hG0,T1,hL1,hG1,T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG);

    // Compute Cp_k and q_k
    cpL = computeCpk(hL0,T0,hL1,T1);  // Liquid
    qL = computeQk(hL0,T0,cpL);
    
    cpG = computeCpk(hG0,T0,hG1,T1);  // Gas
    qG = computeQk(hG0,T0,cpG);

    cout << cpL << " " << qL << " " << endl;
    cout << cpG << " " << qG << endl;

    // Compute pInf_k and gamma_k
    pInfL = computePinfK(vL0,vL1,T0,T1,pSat0,pSat1);
    cvL = computeCvk(cpL,vL0,T0,pSat0,pInfL);
    gammaL = computeGammaK(cpL,cvL);

    pInfG = computePinfK(vG0,vG1,T0,T1,pSat0,pSat1);
    cvG = computeCvk(cpG,vG0,T0,pSat0,pInfG);
    gammaG = computeGammaK(cpG,cvG);

    cout << pInfL << " " << cvL << " " << gammaL << endl;
    cout << pInfG << " " << cvG << " " << gammaG << endl;

    // Compute qPrim_k


    return 0;
}

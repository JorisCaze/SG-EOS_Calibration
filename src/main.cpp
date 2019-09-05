#include <iostream>
#include <iomanip>

#include "tools.h"
#include "functionsRef.h"
#include "functionsShock.h"

using namespace std;

int main()
{
    string run("res/");
    int method(0);

    double c0(0.),v0(0.),p0(0.),a(0.),gaMin(0.),gaMax(0.),ga(0.);
    string fileDfnU("input/DfnU.txt");
    vector<double> d,u,dEstimated,dTheoric;
    // ***
    // double T0,hL0,hG0,T1,hL1,hG1;
    // double T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG;
    // double cpL, qL, cpG, qG, pInfL, cvL, gammaL, pInfG, cvG, gammaG;

    // *** Calibration selection ***
    cout << "Choose your method to calibrate Stiffened Gas Equation Of State \n";
    cout << "(1) Dynamic of Shock wave\n";
    cout << "(2) Liquid and its vapor\n";
    cin >> method;
    // switch (method)
    // {
    // case 1:
    //     runShockMethod();
    //     break;
    // case 2: 
    //     runLiqVapMethod();
    //     break;
    // default:
    //     break;
    // }

    // *** Shock calibration ***
    readShockInput(c0,v0,p0,a,gaMin,gaMax);
    if (a == -1.) {
        readFile(fileDfnU,u,d);
        // a = determineAdiabCoeff1(u,d);
        a = determineAdiabCoeff(u,d,c0);
    }
    for (unsigned int i = 0; i < u.size(); i++) {
        dEstimated.push_back(computeExperimentalShockSpeed(c0,a,u[i]));
    }
    cout << "Dynamic adiabatic coeff. : " << a << endl;
    cout << "Residual : " << residual(d,dEstimated) << endl;

    ga = seekGamma(gaMin,gaMax,c0,u,dEstimated);
    cout << "Gamma : " << ga << endl;
    writeExperimentalPlotFile(u,dEstimated);
    cout << "Pinf : " << computePinfShock(c0,v0,p0,ga) << endl;

    // // *** Liquid/vapor calibration ***
    // // Read reference states
    // readRefStates(T0,hL0,hG0,T1,hL1,hG1,T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG);

    // // Compute Cp_k and q_k
    // cpL = computeCpk(hL0,T0,hL1,T1);  // Liquid
    // qL = computeQk(hL0,T0,cpL);
    
    // cpG = computeCpk(hG0,T0,hG1,T1);  // Gas
    // qG = computeQk(hG0,T0,cpG);

    // cout << cpL << " " << qL << " " << endl;
    // cout << cpG << " " << qG << endl;

    // // Compute pInf_k and gamma_k
    // pInfL = computePinfK(vL0,vL1,T0,T1,pSat0,pSat1);
    // cvL = computeCvk(cpL,vL0,T0,pSat0,pInfL);
    // gammaL = computeGammaK(cpL,cvL);

    // pInfG = computePinfK(vG0,vG1,T0,T1,pSat0,pSat1);
    // cvG = computeCvk(cpG,vG0,T0,pSat0,pInfG);
    // gammaG = computeGammaK(cpG,cvG);

    // cout << pInfL << " " << cvL << " " << gammaL << endl;
    // cout << pInfG << " " << cvG << " " << gammaG << endl;

    // Compute qPrim_k

    return 0;
}

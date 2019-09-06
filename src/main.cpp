#include <iostream>
#include <iomanip>

#include "tools.h"
#include "calib_shock.h"
#include "calib_liq-vap.h"

using namespace std;

int main()
{
    string run("res/");
    int method(0);

    // *** Calibration selection ***
    cout << "Choose your method to calibrate Stiffened Gas Equation Of State \n";
    cout << "(1) Dynamic of Shock wave\n";
    cout << "(2) Liquid and its vapor\n";
    cin >> method;

    switch (method)
    {
    case 1:
    {
        // *** Shock calibration ***
        cout << "*** Shock calibration ***\n";
        double c0(0.),v0(0.),p0(0.),a(0.),gaMin(0.),gaMax(0.),ga(0.);
        string fileDfnU("input/Shock/DfnU.txt");
        vector<double> d,u,dLinearized,dTheoric;
        
        readShockInput(c0,v0,p0,a,gaMin,gaMax);
        if (a == -1.) {
            readFile(fileDfnU,u,d);
            a = determineAdiabCoeff(u,d,c0);
        }
        for (unsigned int i = 0; i < u.size(); i++) {
            dLinearized.push_back(computeExperimentalShockSpeed(c0,a,u[i]));
        }
        cout << "Dynamic adiabatic coeff. : " << a << endl;
        cout << "Residual : " << residual(d,dLinearized) << endl;

        ga = seekGamma(gaMin,gaMax,c0,u,dLinearized);
        cout << "Gamma : " << ga << endl;
        writeExperimentalPlotFile(u,dLinearized);
        cout << "Pinf : " << computePinfShock(c0,v0,p0,ga) << endl;
        break;
    }
    case 2: 
    {
        // *** Liquid/vapor calibration ***
        cout << "*** Liquid/vapor calibration ***\n\n";
        double T0,hL0,hG0,T1,hL1,hG1;
        double T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1,qPrimG;
        double cpL, qL, cpG, qG, pInfL, cvL, gammaL, pInfG, cvG, gammaG;

        // Read reference states
        readRefStates(T0,hL0,hG0,T1,hL1,hG1,T0bis,vL0,vG0,pSat0,T1bis,vL1,vG1,pSat1);

        // Compute Cp_k and q_k
        cpL = computeCpkDM(hL0,T0,hL1,T1);  // Liquid
        qL = computeQk(hL0,T0,cpL);
        
        cpG = computeCpkDM(hG0,T0,hG1,T1);  // Gas
        qG = computeQk(hG0,T0,cpG);

        // Compute pInf_k and gamma_k
        pInfL = computePinfkDM(vL0,vL1,T0,T1,pSat0,pSat1); // Liquid
        cvL = computeCvkDM(cpL,vL0,T0,pSat0,pInfL);
        gammaL = computeGammak(cpL,cvL);

        pInfG = computePinfkDM(vG0,vG1,T0,T1,pSat0,pSat1); // Gas
        cvG = computeCvkDM(cpG,vG0,T0,pSat0,pInfG);
        gammaG = computeGammak(cpG,cvG);

        // Read experimental data files + create tables p,T
        // qPrimG = computeQprimG(p, T,cpL,cpG,cvL,cvG,qL,qG,pInfL,pInfG);

        cout << "-- Liquid (L) ---\n";
        cout << "cpL    (J.kg-1.K-1)  : " << cpL << endl;
        cout << "qL     (J.kg-1)      : " << qL << endl;
        cout << "pInfL  (Pa)          : " << pInfL << endl;
        cout << "cvL    (J.kg-1.K-1)  : " << cvL << endl;
        cout << "gammaL (-)           : " << gammaL << endl;
        cout << "q'L    (J.kg-1)      : " << 0. << endl;

        cout << "\n";

        cout << "-- Gas (G) ---\n";
        cout << "cpG    (J.kg-1.K-1)  : " << cpG << endl;
        cout << "qG     (J.kg-1)      : " << qG << endl;
        cout << "pInfG  (Pa)          : " << pInfG << endl;
        cout << "cvG    (J.kg-1.K-1)  : " << cvG << endl;
        cout << "gammaG (-)           : " << gammaG << endl;
        // cout << "q'G    (J.kg-1)      : " << qPrimG << endl;
        break;
    }
    default:
        cout << "Error : method selection number is undifined\n";
        break;
    }
    return 0;
}

#include <iostream>
#include <string>

#include "tools.h"
#include "calib_shock.h"
#include "calib_liq-vap.h"

using namespace std;

int main()
{
    string run("res/");
    int method(0);

    // *** Calibration selection ***
    displayHeader();                                                                 
    cout << "\nChoose the method to calibrate Stiffened Gas Equation Of State \n";
    cout << "(1) Dynamic of Shock wave\n";
    cout << "(2) Liquid and its vapor\n";
    cout << "Choice : "; cin >> method;
    cout << "\n";

    switch (method)
    {
    case 1:
    {
        // *** Shock calibration ***
        cout << "\n*** Shock calibration ***\n\n";

        double c0(0.),v0(0.),p0(0.),a(0.),gaMin(0.),gaMax(0.),ga(0.);
        string fileAdiabDyn("input/Shock/AdiabDyn.txt");
        vector<double> d,u,dLinearized,dTheoric;
        
        // Reading input file Calib_shock.txt
        readShockInput(c0,v0,p0,a,gaMin,gaMax); 
        
        // Compute dynamic adiabatic coeff.
        if (a == -1.) {
            readFile(fileAdiabDyn,u,d);
            a = determineAdiabCoeff(u,d,c0);
        }

        // Linearize experimental dynamic adiabatic
        for (unsigned int i = 0; i < u.size(); i++) {
            dLinearized.push_back(computeExperimentalShockSpeed(c0,a,u[i]));
        }
        cout << "Dynamic adiabatic coeff. : " << a << endl;
        cout << "Residual : " << residual(d,dLinearized) << endl;

        // Determine gamma with Least Square Method and write dynamic adiabatic associated
        ga = seekGamma(gaMin,gaMax,c0,u,dLinearized);
        cout << "Gamma : " << ga << endl;
        writePlotFile("res/Gamma_x.txt",u,dLinearized);

        // Compute the SG parameter pinf 
        cout << "Pinf : " << computePinfShock(c0,v0,p0,ga) << endl;
        
        break;
    }
    case 2: 
    {
        int calibLiqVap(0);

        // *** Liquid/vapor calibration ***
        cout << "*** Liquid/vapor calibration ***\n\n";

        cout << "Choose the method to do the liquid/vapor calibration :\n";
        cout << "(1) Calibration with two reference states and experimental saturation curve\n";
        cout << "(2) Calibration with one reference state and all experimental curves\n";
        cout << "Choice : "; cin >> calibLiqVap;
        cout << "\n";

        double cpL, qL, cpG, qG, pinfL, cvL, gammaL, pinfG, cvG, gammaG, qPrimL, qPrimG;

        switch (calibLiqVap)
        {
        case 1: // --- Calibration with two reference states and experimental saturation curve ---
        {
            // --- Reference states ---
            double T0,hL0,hG0,T1,hL1,hG1;
            double T0bisL,vL0,pSat0L,T0bisG,vG0,pSat0G;
            double T1bisL,vL1,pSat1L,T1bisG,vG1,pSat1G;
            
            // --- Experimental data ---
            vector<double> pSat,T;

            // Read reference states
            readLiqVapRefStatesDM(T0,hL0,hG0,T1,hL1,hG1,T0bisL,vL0,pSat0L,T0bisG,vG0,pSat0G,T1bisL,vL1,pSat1L,T1bisG,vG1,pSat1G);
            
            // Read experimental saturation curve
            readFile("input/Liq-vap/Psat_exp.txt",T,pSat);

            // --- Liquid ---
            cpL = computeCpkDM(hL0,T0,hL1,T1);
            qL = computeQk(hL0,T0,cpL);
            pinfL = computePinfkDM(vL0,vL1,T0bisL,T1bisL,pSat0L,pSat1L); 
            cvL = computeCvkDM(cpL,vL0,T0bisL,pSat0L,pinfL);
            gammaL = computeGammak(cpL,cvL);
            qPrimL = 0.; // Ref. energy convention

            // --- Vapor ---
            cpG = computeCpkDM(hG0,T0,hG1,T1);
            qG = computeQk(hG0,T0,cpG);            
            pinfG = computePinfkDM(vG0,vG1,T0bisG,T1bisG,pSat0G,pSat1G); 
            cvG = computeCvkDM(cpG,vG0,T0bisG,pSat0G,pinfG);
            gammaG = computeGammak(cpG,cvG);
            qPrimG = computeQprimG(pSat,T,cpL,cpG,cvL,cvG,qL,qG,pinfL,pinfG);

            break;
        }
        case 2: // --- Calibration with one reference state and all experimental curves ---
        {
            double p0,ro0,c0;
            double mT, mhL, mhG;
            vector<double> Texp,PsatExp,vGexp,vLexp,hGexp,hLexp,LvExp;

            // Read reference state
            readRefStateLSM(p0,ro0,c0);

            // Read experimental curves            
            readExpDataLSM("input/Liq-vap/expData.txt",Texp,PsatExp,vGexp,vLexp,hGexp,hLexp,LvExp);
            
            mT = meanValue(Texp);
            mhL = meanValue(hLexp);
            mhG = meanValue(hGexp);

            // --- Liquid ---
            cpL = computeCpkLSM(hLexp,Texp);
            qL = computeQk(mhL,mT,cpL);
            pinfL = computePinfkLSM(PsatExp,Texp,vLexp,cpL,p0,ro0,c0);
            // pinfL = 4.e8;
            cvL = cpL - computeHeatCapDiffkLSM(PsatExp,Texp,vLexp,pinfL);
            gammaL = computeGammak(cpL,cvL);
            qPrimL = 0.; // Ref. energy convention

            // --- Vapor --- 
            cpG = computeCpkLSM(hGexp,Texp);
            qG = computeQk(mhG,mT,cpG);
            cvG = computeCvgLSM(Texp,PsatExp,vGexp,cpG);
            gammaG = computeGammak(cpG,cvG);
            pinfG = 0.;  // Ideal Gas
            qPrimG = computeQprimG(PsatExp,Texp,cpL,cpG,cvL,cvG,qL,qG,pinfL,pinfG);
            
            break;
        }
        default:
            cout << "Error : liquid-vapor method selection number is undifined\n";
            break;
        }        
       
        // Results
        cout << "* Results *\n";
        cout << "-- Liquid (L) ---\n";
        cout << "cpL    (J.kg-1.K-1)  : " << cpL << endl;
        cout << "qL     (J.kg-1)      : " << qL << endl;
        cout << "pinfL  (Pa)          : " << pinfL << endl;
        cout << "cvL    (J.kg-1.K-1)  : " << cvL << endl;
        cout << "gammaL (-)           : " << gammaL << endl;
        cout << "q'L    (J.kg-1)      : " << qPrimL << endl;
        cout << "\n";

        cout << "-- Gas (G) ---\n";
        cout << "cpG    (J.kg-1.K-1)  : " << cpG << endl;
        cout << "qG     (J.kg-1)      : " << qG << endl;
        cout << "pinfG  (Pa)          : " << pinfG << endl;
        cout << "cvG    (J.kg-1.K-1)  : " << cvG << endl;
        cout << "gammaG (-)           : " << gammaG << endl;
        cout << "q'G    (J.kg-1)      : " << qPrimG << endl;
        cout << "\n";

        // Plot theoric curves
        vector<double> Tth, hLth, hGth, LvTh, PsatTh, vlTh, vgTh;
        int Nth(1000);
        double dT,Tinit;
        double A(0.),B(0.),C(0.),D(0.),Tmin,Tmax;

        cout << "Define the temperature interval of study\n";
        cout << "Tmin : "; cin >> Tmin;
        cout << "Tmax : "; cin >> Tmax;
        dT = fabs(Tmax-Tmin)/Nth; Tinit = fmin(Tmin,Tmax);
 
        coeffPsatTh(cpG,cpL,cvG,cvL,qG,qL,qPrimG,A,B,C,D);
        for (int i = 0; i < Nth; i++) {
            Tth.push_back(Tinit);
            hLth.push_back(computeThEnthalpy(cpL,qL,Tth[i]));
            hGth.push_back(computeThEnthalpy(cpG,qG,Tth[i]));
            LvTh.push_back(hGth[i]-hLth[i]);
            PsatTh.push_back(computePsatTh(A,B,C,D,pinfG,pinfL,Tth[i]));
            vlTh.push_back(computeVkTh(cpL,cvL,pinfL,Tth[i],PsatTh[i]));
            vgTh.push_back(computeVkTh(cpG,cvG,pinfG,Tth[i],PsatTh[i]));
            Tinit += dT;
        }
        writePlotFile("res/hL_th.txt",Tth,hLth);
        writePlotFile("res/hG_th.txt",Tth,hGth);
        writePlotFile("res/Lv_th.txt",Tth,LvTh);
        writePlotFile("res/Psat_th.txt",Tth,PsatTh);
        writePlotFile("res/vL_th.txt",Tth,vlTh);
        writePlotFile("res/vG_th.txt",Tth,vgTh);

        break;
    }
    default:
        cout << "Error : method selection number is undifined\n";
        break;
    }
    cout << "\n";
    return 0;
}

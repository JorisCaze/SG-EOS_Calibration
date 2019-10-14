#include <iostream>
#include <string>
#include <algorithm>

#include "tools.h"
#include "calib_shock.h"
#include "calib_liq-vap.h"

using namespace std;

int main()
{
    string run("res/");
    int calibration(0), liVapMethod(0);
    double tpMin(0.), tpMax(0.);   // Temperatures plot interval
    double tpStart(0.), tpEnd(0.); // Temperatures interval for enthalpies calibration in liq/vap calibration LSM 

    // *** Calibration selection ***
    displayHeader();
    readInput(calibration,liVapMethod);

    switch (calibration)
    {
    case 1:
    {
        // *** Shock calibration ***
        cout << "\n*** Shock calibration ***\n\n";

        double c0(0.),v0(0.),p0(0.),a(0.),gaMin(0.),gaMax(0.),ga(0.),dMin(0.),dMax(0.);
        string fileAdiabDyn("input/Shock/AdiabDyn.txt");
        vector<double> dExp,u,dLinearized,dTheoric;
        double uMin(0.),uMax(0.),du(0.);

        // Reading input file Calib_shock.txt
        readShockInput(c0,v0,p0,a,gaMin,gaMax,dMin,dMax); 
        
        // Compute/read dynamic adiabatic coeff.
        if (a == -1.) { // Exp. file available
            readFile(fileAdiabDyn,u,dExp);
            a = determineAdiabCoeff(u,dExp,c0);
            writePlotFile("res/Gamma-exp.txt",u,dExp);
        }
        else { // No exp. file -> a is known
            uMin = (dMin - c0)/a;
            uMax = (dMax - c0)/a;
            du = (uMax - uMin)/100.;
            for (unsigned int i = 0; i < 100; i++) {
                u.push_back(uMin + i*du);
            }
        }
        cout << "Dynamic adiabatic coeff. : " << a << endl;

        // Linearize experimental dynamic adiabatic
        for (unsigned int i = 0; i < u.size(); i++) {
            dLinearized.push_back(computeExperimentalShockSpeed(c0,a,u[i]));
        }
        
        // cout << "Residual : " << residual(dExp,dLinearized) << endl;

        // Determine gamma with Least Square Method and write dynamic adiabatic associated
        ga = seekGamma(gaMin,gaMax,c0,u,dLinearized);
        cout << "Gamma : " << ga << endl;
        writePlotFile("res/Gamma-expLin.txt",u,dLinearized);

        // Compute the SG parameter pinf 
        cout << "Pinf : " << computePinfShock(c0,v0,p0,ga) << endl;
        
        break;
    }
    case 2: 
    {
        // *** Liquid/vapor calibration ***
        cout << "*** Liquid/vapor calibration ***\n\n";

        double cpL(0.), qL(0.), cpG(0.), qG(0.), pinfL(0.), cvL(0.), gammaL(0.), pinfG(0.), cvG(0.), gammaG(0.), qPrimL(0.), qPrimG(0.);

        switch (liVapMethod)
        {
        case 1: // --- DM Calibration with two reference states and experimental saturation curve ---
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
            readFile("input/Liq-vap/DM/Psat_exp.txt",T,pSat);
            string fileName[6] = {"hG_exp.txt","hL_exp.txt","Lv_exp.txt","Psat_exp.txt","vG_exp.txt","vL_exp.txt"};
            for (int i = 0; i < 6; i++) {
                mvFileToRes("input/Liq-vap/DM/",fileName[i]);   
            }

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
            if (cvG < 0.) 
                cvG = computeCvkDM(cpG,vG1,T1bisG,pSat1G,pinfG);
            gammaG = computeGammak(cpG,cvG);
            qPrimG = computeQprimG(pSat,T,cpL,cpG,cvL,cvG,qL,qG,pinfL,pinfG);

            tpMin = *min_element(T.begin(), T.end());
            tpMax = *max_element(T.begin(), T.end());

            break;
        }
        case 2: // --- LSM Calibration with one reference state and all experimental curves ---
        {
            double p0,ro0,c0;
            double mTfhk, mhL, mhG;
            vector<double> Texp,PsatExp,vGexp,vLexp,hGexp,hLexp,LvExp;

            // Read reference state
            readRefStateLSM(p0,ro0,c0,tpStart,tpEnd);

            // Read experimental curves            
            readExpDataLSM("input/Liq-vap/LSM/expData.txt",Texp,PsatExp,vGexp,vLexp,hGexp,hLexp,LvExp);
            unsigned int istart(0), iend(Texp.size()); // Index for enthalpies fitting 
            
            // Split expData into single files
            writePlotFile(run+"Psat_exp.txt",Texp,PsatExp);
            writePlotFile(run+"vG_exp.txt",Texp,vGexp);
            writePlotFile(run+"vL_exp.txt",Texp,vLexp);
            writePlotFile(run+"hG_exp.txt",Texp,hGexp);
            writePlotFile(run+"hL_exp.txt",Texp,hLexp);
            writePlotFile(run+"Lv_exp.txt",Texp,LvExp);

            // Select interval of temperatures for LSM on enthalpies
            if (tpStart > 0){
                for(unsigned int i=0; i < Texp.size(); i++){
                    if(Texp[i]>tpStart) {istart = i; break;}
                }
            }
            if (tpEnd > 0){
                for(unsigned int i=0; i < Texp.size(); i++){
                    if(Texp[i]>tpEnd) {iend = i; break;}
                }
            }

            mTfhk = meanValue(Texp,istart,iend);
            mhL = meanValue(hLexp,istart,iend);
            mhG = meanValue(hGexp,istart,iend);

            // --- Liquid ---
            cpL = computeCpkLSM(hLexp,Texp,istart,iend);
            qL = computeQk(mhL,mTfhk,cpL);
            pinfL = computePinfkLSM(PsatExp,Texp,vLexp,cpL,p0,ro0,c0);
            cvL = cpL - computeHeatCapDiffkLSM(PsatExp,Texp,vLexp,pinfL);
            gammaL = computeGammak(cpL,cvL);
            qPrimL = 0.; // Ref. energy convention

            // // --- Vapor --- 
            cpG = computeCpkLSM(hGexp,Texp,istart,iend);
            qG = computeQk(mhG,mTfhk,cpG);
            cvG = computeCvgLSM(Texp,PsatExp,vGexp,cpG);
            gammaG = computeGammak(cpG,cvG);
            pinfG = 0.;  // Ideal Gas
            qPrimG = computeQprimG(PsatExp,Texp,cpL,cpG,cvL,cvG,qL,qG,pinfL,pinfG);

            // Range of temperature for plot
            tpMin = *min_element(Texp.begin(), Texp.end());
            tpMax = *max_element(Texp.begin(), Texp.end());
            
            break;
        }
        default:
            cout << "Error : liquid-vapor method selection number is undifined\n";
            break;
        }        
       
        // Results
        cout << "* Results *\n";
        cout << "--- Liquid (L) ---\n";
        cout << "cpL    (J.kg-1.K-1)  : " << cpL << endl;
        cout << "qL     (J.kg-1)      : " << qL << endl;
        cout << "pinfL  (Pa)          : " << pinfL << endl;
        cout << "cvL    (J.kg-1.K-1)  : " << cvL << endl;
        cout << "gammaL (-)           : " << gammaL << endl;
        cout << "q'L    (J.kg-1)      : " << qPrimL << endl;
        cout << "\n";

        cout << "--- Gas (G) ---\n";
        cout << "cpG    (J.kg-1.K-1)  : " << cpG << endl;
        cout << "qG     (J.kg-1)      : " << qG << endl;
        cout << "pinfG  (Pa)          : " << pinfG << endl;
        cout << "cvG    (J.kg-1.K-1)  : " << cvG << endl;
        cout << "gammaG (-)           : " << gammaG << endl;
        cout << "q'G    (J.kg-1)      : " << qPrimG << endl;
        cout << "\n";

        // Plot theoric curves
        vector<double> Tth, hLth, hGth, LvTh, PsatTh, vlTh, vgTh;
        int Nth(500);
        double dT,Tinit;
        double A(0.),B(0.),C(0.),D(0.);

        dT = fabs(tpMax-tpMin)/Nth; Tinit = fmin(tpMin,tpMax);
 
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

#include "calib_liq-vap.h"
#include "tools.h"

using namespace std;

void readLiqVapInput(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bisL, double &vL0, double &pSat0L, double &T0bisG, double &vG0, double &pSat0G, double &T1bisL, double &vL1, double &pSat1L, double &T1bisG, double &vG1, double &pSat1G)
{
    ifstream strmRefStates("input/Calib_liq-vap.txt");
    string line("");
    if (strmRefStates) {
        for (int i=1; i<8; i++) {getline(strmRefStates,line);}
        T0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        hL0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        hG0 = stod(line);

        for (int i=1; i<5; i++) {getline(strmRefStates,line);}        
        T1 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        hL1 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        hG1 = stod(line);

        for (int i=1; i<7; i++) {getline(strmRefStates,line);}        
        T0bisL = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vL0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        pSat0L = stod(line);

        for (int i = 1; i < 4; i++) {getline(strmRefStates,line);}
        T0bisG = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vG0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        pSat0G = stod(line);

        for (int i=1; i<6; i++) {getline(strmRefStates,line);}        
        T1bisL = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vL1 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        pSat1L = stod(line);

        for (int i = 1; i < 4; i++) {getline(strmRefStates,line);}
        T1bisG = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vG1 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        pSat1G = stod(line);
    }
    else {
        cout << "Error : reading Calib_liq-vap.txt file\n"; exit(0);
    }
}

// **************************************************

double computeCpkDM(double hk0, double T0, double hk1, double T1)
{
    // Purpose : compute heat capacity at constant pressure of phase k with reference states 0 and 1
    // See equations (49-50) 
    return (hk1-hk0)/(T1-T0);
}

double computePinfkDM(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1)
{
    // Purpose : compute parameter pInfK of phase k with reference states 0 and 1
    // See equations (61-62) 
    double pinf((vk0*T1*pSat0-vk1*T0*pSat1)/(vk1*T0-vk0*T1));
    if (pinf < 1.e-1)
        return 0.;
    else
        return pinf;    
}

double computeCvkDM(double cpk, double vk0, double T0, double pSat0, double pInfK)
{
    // Purpose : compute heat capacity at constant volume of phase k with reference states 0
    // More : if used for ideal gas, input pInfK = 0 
    // See equations (65-66) 
    return (cpk-(vk0/T0)*(pSat0+pInfK));
}

// **************************************************

double computeCpkLSM(vector<double> hkExp, vector<double> Texp)
{
    // Purpose : compute heat capacity at constant pressure of phase k with LSM
    // More : the exp. data hk(Tk) is used 
    // See equation (50) of Le Métayer, O., & Saurel, R. (2016). The noble-abel stiffened-gas equation of state. Physics of Fluids, 28(4), 046102.
    double mHk, mT; // Mean values from experimental data tables
    double num(0.), den(0.);
    mHk = meanValue(hkExp);
    mT = meanValue(Texp);
    for (unsigned int i = 0; i < hkExp.size(); i++) {
        num += (Texp[i]*(hkExp[i]-mHk));
        den += (Texp[i]*(Texp[i]-mT));
    }
    return num/den;
}

// **************************************************

double computeQk(double hk, double T, double cpk)
{
    // Purpose : compute formation energy of phase k 
    // Use 1 : Can be used with reference state (hk0,T0) for the Differential Method
    // See equations (51-52) 
    // Use 2 : Can be used with Least Squares Method with input (mean(hk),mean(Tk))
    // See (51) 
    return (hk-cpk*T);
}

double computeGammak(double cpk, double cvk)
{
    // Purpose : compute adiabatic index
    // See equation (67) 
    return cpk/cvk;
}

double computeQprimG(vector<double> p, vector<double> T, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG)
{
    // Purpose : compute the liquid entropy constant with LSM following the step of Le Métayer, O., & Saurel, R. (2016). The noble-abel stiffened-gas equation of state. Physics of Fluids, 28(4), 046102.
    // See equation (70) 
    double A,B,C,D,sum(0.);
    B = (qL-qG)/(cpG-cvG);
    C = (cpG-cpL)/(cpG-cvG);
    D = (cpL-cvL)/(cpG-cvG);
    for (unsigned int i = 0; i < p.size(); i++) {
        sum += log(p[i]+pInfG)-B/T[i]-C*log(T[i])-D*log(p[i]+pInfL);
    }
    A = sum/p.size();
    return (A*(cpG-cvG)+cpG-cpL);
}

// **************************************************

void coeffPsatTh(double cpG, double cpL, double cvG, double cvL, double qG, double qL, double qPrimG, double &A, double &B, double &C, double &D)
{
    // Purpose : compute coeff. of theoric saturation curve Psat(T)
    // See equation 70 
    A = (cpL-cpG+qPrimG)/(cpG-cvG);
    B = (qL-qG)/(cpG-cvG);
    C = (cpG-cpL)/(cpG-cvG);
    D = (cpL-cvL)/(cpG-cvG);
}

double computeThEnthalpy(double cpk, double qk, double T)
{
    // Purpose : compute phasic theoric enthalpy 
    // See equations (45)-(46) [1
    return (cpk*T+qk);
}

double computePsatTh(double A, double B, double C, double D, double pinfG, double pinfL, double T)
{
    // Purpose : compute saturated pressure Psat at a given temperature
    // See equation (70)  
    // More : Newton-Raphson algo. is used
    double fp, dfp, p1(1.e5), p2(0.), eps(1.);
    int count(0);

    while (eps > 1.e-5 && count < 50) {
        fp = log(p1+pinfG) - A - B/T - C*log(T) - D*log(p1+pinfL);
        dfp = 1./(p1+pinfG) - D/(p1+pinfL);
        p2 = p1 - fp/dfp;
        eps = fabs(p2-p1)/(0.5*(p1+p2));
        p1 = p2;
        count++;
        if (count >= 50) 
            cout << "Warning : newton-raphson of Psat(T) function not converged\n";
    }
    if (p1 < 1.e-3)
        return 0.;
    else
        return p1;
}

double computeVkTh(double cpk, double cvk, double pinfk, double T, double Psat)
{
    // Purpose : compute specific vol. of a phase k
    // See equation (55) and (56) 
    return ((cpk-cvk)*T)/(Psat+pinfk);
}

// **************************************************

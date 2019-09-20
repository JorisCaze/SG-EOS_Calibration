#include "calib_liq-vap.h"
#include "tools.h"

using namespace std;

void readLiqVapRefStatesDM(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bisL, double &vL0, double &pSat0L, double &T0bisG, double &vG0, double &pSat0G, double &T1bisL, double &vL1, double &pSat1L, double &T1bisG, double &vG1, double &pSat1G)
{
    ifstream strmRefStates("input/Liq-vap/DM/Calib_liq-vap_DM.txt");
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
        cout << "Error : reading Calib_liq-vap_DM.txt file\n"; exit(0);
    }
}

void readExpDataLSM(string file, vector<double> &Texp, vector<double> &PsatExp, vector<double> &vGexp, vector<double> &vLexp, vector<double> &hGexp, vector<double> &hLexp, vector<double> &LvExp)
{
    // Purpose : read the experimental data with 7 columns and an ignored header
    ifstream strmIn(file.c_str());
    string line; 
    double dat1, dat2, dat3, dat4, dat5, dat6, dat7;
    if (strmIn) {
        while (getline(strmIn,line)) {
            strmIn >> dat1 >> dat2 >> dat3 >> dat4 >> dat5 >> dat6 >> dat7;
            Texp.push_back(dat1);
            PsatExp.push_back(dat2);
            vGexp.push_back(dat3);
            vLexp.push_back(dat4);
            hGexp.push_back(dat5);
            hLexp.push_back(dat6);
            LvExp.push_back(dat7);
        }
    }
    else {
        cout << "Error : reading experimental data file " << file << ".txt\n"; exit(0);
    }
}

void readRefStateLSM(double &p0, double &ro0, double &c0)
{
    ifstream strmRefStates("input/Liq-vap/LSM/Calib_liq-vap_LSM.txt");
    string line("");
    if (strmRefStates) {
        for (int i=1; i<5; i++) {getline(strmRefStates,line);}
        p0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        ro0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        c0 = stod(line);
    }
    else {
        cout << "Error : reading Calib_liq-vap_LSM.txt file\n"; exit(0);
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

double computeCpkLSM(vector<double> const &hkExp, vector<double> const &Texp)
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

double computeCvgLSM(vector<double> const &Texp, vector<double> const &PsatExp, vector<double> const &vGexp, double cpG)
{
    // Purpose : compute gas heat cap. at cst vol. with LSM
    // More : the assumption pinfG = 0 is done here, the vapor is an IG
    // See eq. (55) of Le Métayer, O., & Saurel, R. (2016). The noble-abel stiffened-gas equation of state. Physics of Fluids, 28(4), 046102.
    double num(0.), den(0.);
    for (unsigned int i = 0; i < Texp.size(); i++) {
        num += vGexp[i]*Texp[i]/PsatExp[i];
        den += (Texp[i]/PsatExp[i])*(Texp[i]/PsatExp[i]);
    }
    return (cpG - num/den);
}

double computeHeatCapDiffkLSM(vector<double> const& PsatExp, vector<double> const& Texp, vector<double> const& vKexp, double pinfk)
{
    // Purpose : compute heat capacity cpK - cvK of phase k 
    // More : this fn is also used during the Newton-Raphson iterative process of computePinfkLSM()
    double num(0.), den(0.);
    for (unsigned int i = 0; i < Texp.size(); i++) {
        num += (Texp[i]*vKexp[i])/(PsatExp[i]+pinfk);
        den += (Texp[i]/(PsatExp[i]+pinfk))*(Texp[i]/(PsatExp[i]+pinfk));
    }
    return num/den;
}

double computeDHeatCapDiffkLSM(vector<double> const& PsatExp, vector<double> const& Texp, vector<double> const& vKexp, double pinfk)
{
    // Purpose : compute pinfK derivative of fn computeHeatCapDiffkLSM() for computePinfkLSM Newton-Raphson process
    double bf1(0.), bf2(0.), a1(0.), a2(0.), s1(0.), s2(0.);
    for (unsigned int i = 0; i < Texp.size(); i++) {
        bf1 = Texp[i]/(PsatExp[i]+pinfk);
        bf2 = bf1/(PsatExp[i]+pinfk);
        a1 += -vKexp[i]*bf2;
        a2 += -2.*bf1*bf2;
        s1 += bf1*bf1;
        s2 += vKexp[i]*bf1;
    }
    return (a1*s1-s2*a2)/(s1*s1);
}

double computePinfkLSM(vector<double> const &PsatExp, vector<double> const &Texp, vector<double> const &vKexp, double cp, double p0, double ro0, double c0)
{
    // Purpose : compute pinfk parameter using the Newton-Raphson procedure
    // More : use a ref. state following the method described in (68) of 
    //        Le Métayer, O., & Saurel, R. (2016). The noble-abel stiffened-gas equation of state. Physics of Fluids, 28(4), 046102.
    double fp, dfp, pinf1(5.e5), pinf2(0.), err(1.);
    double diffC, dDiffC;
    int count(0);

    while (err > 1.e-5 && count < 100) {
        diffC = computeHeatCapDiffkLSM(PsatExp,Texp,vKexp,pinf1);
        dDiffC = computeDHeatCapDiffkLSM(PsatExp,Texp,vKexp,pinf1);

        fp = p0 + pinf1 - ro0*c0*c0*(1.-diffC/cp);
        dfp = 1. + ro0*c0*c0*(dDiffC)/cp;

        pinf2 = pinf1 - fp/dfp;
        err = fabs(pinf2-pinf1)/(0.5*(pinf1+pinf2));
        pinf1 = pinf2;
        count++;
        if (count >= 100) {
            cout << "Warning : newton-raphson of Psat(T) function not converged\n"; exit(0);
        }
    }

    // cout << "Number of iterations of pinfL NR : " << count << endl;
    if (pinf2 < 1.e-6)
        return 0.;
    else 
        return pinf1;
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

double computeQprimG(vector<double> const &p, vector<double> const &T, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG)
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
    // See equations (45)-(46)
    return (cpk*T+qk);
}

double computePsatTh(double A, double B, double C, double D, double pinfG, double pinfL, double T)
{
    // Purpose : compute saturated pressure Psat at a given temperature
    // See equation (70), fp is obtained with exp((70))
    // More : Newton-Raphson algo. is used
    double fp, dfp, p1(1.e5), p2(0.), err(1.);
    int count(0);

    while (err > 1.e-5 && count < 50) {
        // fp = log(p1+pinfG) - A - B/T - C*log(T) - D*log(p1+pinfL);
        // dfp = 1./(p1+pinfG) - D/(p1+pinfL);
        fp = p1 + pinfG - exp(A+B/T+C*log(T)) * pow((p1+pinfL),D);
        dfp = 1. - exp(A+B/T+C*log(T))*D*pow((p1+pinfL),D-1.);
        p2 = p1 - fp/dfp;
        err = fabs(p2-p1)/(0.5*(p1+p2));
        p1 = p2;
        count++;
        if (count >= 50) 
            cout << "Warning : newton-raphson of Psat(T) function not converged\n";
    }
    if (p2 < 1.e-6)
        return 0.;
    else
        return p2;
}

double computeVkTh(double cpk, double cvk, double pinfk, double T, double Psat)
{
    // Purpose : compute specific vol. of a phase k
    // See equation (55) and (56) 
    return ((cpk-cvk)*T)/(Psat+pinfk);
}

// **************************************************

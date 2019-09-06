#include "calib_liq-vap.h"
#include "tools.h"

using namespace std;

void readRefStates(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bis, double &vL0, double &vG0, double &pSat0, double &T1bis, double &vL1, double &vG1, double &pSat1)
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

        for (int i=1; i<6; i++) {getline(strmRefStates,line);}        
        T0bis = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vL0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vG0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        pSat0 = stod(line);

        for (int i=1; i<5; i++) {getline(strmRefStates,line);}        
        T1bis = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vL1 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        vG1 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        pSat1 = stod(line);
    }
    else {
        cout << "Error : reading Calib_liq-vap.txt file\n"; exit(0);
    }
}


void readDoubleFile(string const &file1, string const &file2, vector<double> &tab1, vector<double> &T_tab1, vector<double> &tab2, vector<double> &T_tab2)
{
    // Purpose : read two files and fill tables
    ifstream streamFile1(file1), streamFile2(file2);
    string line;
    double data1(0.), data2(0.);

    if (streamFile1) {
        while (getline(streamFile1,line)) {
            streamFile1 >> data1 >> data2; 
            tab1.push_back(data1);
            T_tab1.push_back(data2);
        }
    } 
    else {
        cout << "Error : reading experimental data file " << file1 << ".txt\n"; exit(0);
    }

    if (streamFile2) {
        while (getline(streamFile2,line)) {
            streamFile2 >> data1 >> data2; 
            tab2.push_back(data1);
            T_tab2.push_back(data2);
        }
    } 
    else {
        cout << "Error : reading experimental data file " << file2 << ".txt\n"; exit(0);
    }
}

// **************************************************

double computeCpkDM(double hk0, double T0, double hk1, double T1)
{
    // Purpose : compute heat capacity at constant pressure of phase k with reference states 0 and 1
    // See equations (49-50) [1]
    return (hk1-hk0)/(T1-T0);
}

double computePinfkDM(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1)
{
    // Purpose : compute parameter pInfK of phase k with reference states 0 and 1
    // See equations (61-62) [1]
    return (vk0*T1*pSat0-vk1*T0*pSat1)/(vk1*T0-vk0*T1);
}

double computeCvkDM(double cpk, double vk0, double T0, double pSat0, double pInfK)
{
    // Purpose : compute heat capacity at constant volume of phase k with reference states 0
    // More : if used for ideal gas, input pInfK = 0 
    // See equations (65-66) [1]
    return cpk-(vk0/T0)*(pSat0+pInfK);
}

// **************************************************

double computeCpkLSM(vector<double> hkExp, vector<double> Texp)
{
    // Purpose : compute heat capacity at constant pressure of phase k with LSM
    // More : the exp. data hk(Tk) is used 
    // See equation (50) [2]
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
    // See equations (51-52) [1]
    // Use 2 : Can be used with Least Squares Method with input (mean(hk),mean(Tk))
    // See (51) [2]
    return (hk-cpk*T);
}

double computeGammak(double cpk, double cvk)
{
    // Purpose : compute adiabatic index
    // See equation (67) [1]
    return cpk/cvk;
}

double computeQprimG(std::vector<double> p, std::vector<double> T, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG)
{
    // Purpose : compute the lqiuid entropy constant with LSM following [2] step
    // See equation (70) [1]
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
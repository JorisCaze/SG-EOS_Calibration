#include "functionsRef.h"

using namespace std;

void readRefStates(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bis, double &vL0, double &vG0, double &pSat0, double &T1bis, double &vL1, double &vG1, double &pSat1, double &qPrimG)
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

        for (int i=1; i<5; i++) {getline(strmRefStates,line);}        
        qPrimG = stod(line);
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

double computeCpk(double hk0, double T0, double hk1, double T1)
{
    // Purpose : compute heat capacity at constant pressure of phase k with reference states 0 and 1
    // See equations (49-50)
    return (hk1-hk0)/(T1-T0);
}

double computeQk(double hk0, double T0, double cpk)
{
    // Purpose : compute formation energy of phase k with reference states 0
    // See equations (51-52)
    return (hk0-cpk*T0);
}

double computePinfK(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1)
{
    // Purpose : compute parameter pInfK of phase k with reference states 0 and 1
    // See equations (61-62)
    return (vk0*T1*pSat0-vk1*T0*pSat1)/(vk1*T0-vk0*T1);
}

double computeCvk(double cpk, double vk0, double T0, double pSat0, double pInfK)
{
    // Purpose : compute heat capacity at constant volume of phase k with reference states 0
    // See equations (65-66)
    return cpk-(vk0/T0)*(pSat0+pInfK);
}

double computeGammaK(double cpk, double cvk)
{
    // Purpose : compute adiabatic index
    // See equation (67)
    return cpk/cvk;
}

// double fnPstar(double p, double T, double qPrimG, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG)
// {
//     double A(0.), B(0.), C(0.), D(0.), fPstar(0.);
//     A = (cpL-cpG+qPrimG)/(cpG-cvG);
//     B = (qL-qG)/(cpG-cvG);
//     C = (cpG-cpL)/(cpG-cvG);
//     D = (cpL-cvL)/(cpG-cvG);
//     fPstar = log(p+pInfG) - A - B/T - C*log(T) - D*log(p+pInfL);
//     return fPstar;
// }

// double dichoPstar(double T, double qPrimG, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG)
// {

// }

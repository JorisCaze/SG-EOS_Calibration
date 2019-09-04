#include "functionsShock.h"

using namespace std;

void readShockInput(double &c0, double &v0, double &a, double &gaMin, double &gaMax)
{
    ifstream strmRefStates("input/shock.txt");
    string line("");
    if (strmRefStates) {
        for (int i=1; i<7; i++) {getline(strmRefStates,line);}
        c0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        v0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        a = stod(line);

        for (int i=1; i<5; i++) {getline(strmRefStates,line);}        
        gaMin = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        gaMax = stod(line);
    }
    else {
        cout << "Error : reading shock.txt file\n"; exit(0);
    }
}

void readFile(string const &file1, vector<double> &tab_x,vector<double> &tab_y)
{
    ifstream streamFile1(file1);
    string line;
    double data1(0.), data2(0.);

    if (streamFile1) {
        while (getline(streamFile1,line)) {
            streamFile1 >> data1 >> data2; 
            tab_x.push_back(data1);
            tab_y.push_back(data2);
        }
    } 
    else {
        cout << "Error : reading experimental data file " << file1 << ".txt\n"; exit(0);
    }
}

double determineAdiabCoeff(vector<double> const &tab_x, vector<double> const &tab_y)
{
    double a(0.);
    a = (tab_y.back()-tab_y[0])/(tab_x.back()-tab_x[0]);
    return a;
}

double computeShockSpeed(double c0, double gamma, double u)
{
    // Purpose : compute shock speed for a material speed u at a specific gamma
    // See equation (43) 
    return (sqrt(c0*c0+(gamma+1.)*u/4)+(gamma+1.)*(u/4.));
}

double computePinf(double c0, double v0, double p0, double gamma)
{
    // Purpose : compute SG parameter pInf
    // See equation (44)
    return ((c0*c0)/(gamma*v0)-p0);
}


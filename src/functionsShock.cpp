#include "functionsShock.h"

using namespace std;

void readShockInput(double &c0, double &v0, double &a, double &gaMin, double &gaMax)
{
    // Purpose : read shock.txt input file for shock calibration
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

void readFile(string const &file, vector<double> &tab_x,vector<double> &tab_y)
{
    // Purpose : read experimental data file with two columns and a header (ignored)
    ifstream streamFile(file);
    string line;
    double data1(0.), data2(0.);

    if (streamFile) {
        while (getline(streamFile,line) && line != "\n") {
            streamFile >> data1 >> data2; 
            tab_x.push_back(data1);
            tab_y.push_back(data2);
        }
    } 
    else {
        cout << "Error : reading experimental data file " << file << ".txt\n"; exit(0);
    }
}

double determineAdiabCoeff(vector<double> const &tab_x, vector<double> const &tab_y)
{
    // Purpose : do the average of adiabatic dynamic coeff. 
    // Var :: a is the table of adiab. dyn. coeff. - A is the mean adiab. dyn. coeff.
    vector<double> a;
    double sum(0.), A(0.);
    if (tab_x.size() > 3) {
        for (int i = 1; i < (static_cast<int>(tab_x.size())/2)+1; i++) {
            a.push_back((tab_y[tab_y.size()-i]-tab_y[i-1])/(tab_x[tab_x.size()-i]-tab_x[i-1]));
        }
        for (unsigned int i = 0; i < a.size(); i++) {
           sum += a[i];
        }
        A = sum/a.size();
    }
    else {
        A = (tab_y.back()-tab_y[0])/(tab_x.back()-tab_x[0]);        
    }
    return A;
}

double computeTheoricShockSpeed(double c0, double gamma, double u)
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


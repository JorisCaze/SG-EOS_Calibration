#include "functionsShock.h"
#include "tools.h"

using namespace std;

void readShockInput(double &c0, double &v0, double &p0, double &a, double &gaMin, double &gaMax)
{
    // Purpose : read shock.txt input file for shock calibration
    ifstream strmRefStates("input/Calib_shock.txt");
    string line("");
    if (strmRefStates) {
        for (int i=1; i<7; i++) {getline(strmRefStates,line);}
        c0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        v0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        p0 = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        a = stod(line);

        for (int i=1; i<5; i++) {getline(strmRefStates,line);}        
        gaMin = stod(line);
        getline(strmRefStates,line); getline(strmRefStates,line);
        gaMax = stod(line);
    }
    else {
        cout << "Error : reading Calib_shock.txt file\n"; exit(0);
    }
}

void readFile(string const &file, vector<double> &tab_x, vector<double> &tab_y)
{
    // Purpose : read experimental data file with two columns and a header (ignored)
    ifstream streamFile(file);
    string line;
    double data1(0.), data2(0.);

    if (streamFile) {
        while (getline(streamFile,line)) {
            streamFile >> data1 >> data2; 
            tab_x.push_back(data1);
            tab_y.push_back(data2);
        }
    } 
    else {
        cout << "Error : reading experimental data file " << file << ".txt\n"; exit(0);
    }
}

// double determineAdiabCoeff(vector<double> const &tab_x, vector<double> const &tab_y)
// {
//     // Purpose : do the average of adiabatic dynamic coeff. 
//     // Var :: a is the table of adiab. dyn. coeff. - A is the mean adiab. dyn. coeff.
//     vector<double> a;
//     double sum(0.), A(0.);
//     if (tab_x.size() > 3) {
//         for (int i = 1; i < (static_cast<int>(tab_x.size())/2)+1; i++) {
//             a.push_back((tab_y[tab_y.size()-i]-tab_y[i-1])/(tab_x[tab_x.size()-i]-tab_x[i-1]));
//         }
//         for (unsigned int i = 0; i < a.size(); i++) {
//            sum += a[i];
//         }
//         A = sum/a.size();
//     }
//     else {
//         A = (tab_y.back()-tab_y[0])/(tab_x.back()-tab_x[0]);        
//     }
//     return A;
// }

double determineAdiabCoeff(vector<double> &tab_x, vector<double> &tab_y, double c0)
{
    // Purpose : do the average of adiabatic dynamic coeff. with Ordinary Lest Squares (OLS) procedure
    // More : c0 is defined as the intercept of fn D = a*u + c0
    return ((meanValue(tab_y)-c0)/meanValue(tab_x));
}

double computeExperimentalShockSpeed(double c0, double a, double u)
{
    // Purpose : compute experimental shock speed according to the dynamic adiabatic linear law 
    // See equation (40)
    return (c0+a*u);
}

void writeExperimentalPlotFile(vector<double> &u, vector<double> &dEstimated)
{
    // Purpose : write the linearized experimental data 
    string fileExp("res/Gamma_x.txt");
    ofstream strmExp(fileExp.c_str());
    for (unsigned int i = 0; i < u.size(); i++) {
        strmExp << u[i] << " " << dEstimated[i] << endl;
    }
}

double seekGamma(double gaMin, double gaMax, double c0, vector<double> &u, vector<double> &dEstimated)
{
    // Purpose : find the best adiabatic index gamma from a range of gamma data with with Ordinary Lest Squares (OLS) procedure and plot the curve D=f(u,gamma_i)
    // More : at each gamma_i the shock speed D=f(u,gamma_i) is evaluated and the residual is computed with the linearized experimental curve
    double ga(0.),gaFit(0.),resd(0.);
    vector<double> dTheoric(u.size(),0.);
    int k(0);
    
    if (gaMin<gaMax) {
        ga = floor(gaMin);
        while(ga<=gaMax) {
            string fileGama("res/Gamma_" + toStrShort(ga) + ".txt");
            ofstream strmShock(fileGama.c_str());
            for (unsigned int i = 0; i < u.size(); i++) {
                dTheoric[i] = computeTheoricShockSpeed(c0,ga,u[i]);
                strmShock << u[i] << " " << dTheoric[i] << endl;
            }
            if (residual(dEstimated,dTheoric) > resd) {
                resd = residual(dEstimated,dTheoric);
                gaFit = ga;
            }
            ga += 0.5;
            k++;
        }
        return gaFit; 
    }
    else {
        cout << "Error : adiabatic index gamma\n";
        return 0;
    }
}

double computeTheoricShockSpeed(double c0, double gamma, double u)
{
    // Purpose : compute shock speed for a material speed u at a specific gamma
    // See equation (43) 
    return (sqrt(c0*c0+(gamma+1.)*u/4)+(gamma+1.)*(u/4.));
}

double computePinfShock(double c0, double v0, double p0, double gamma)
{
    // Purpose : compute SG parameter pInf
    // See equation (44)
    if ((c0*c0)/(gamma*v0)-p0>10.e-6)
        return ((c0*c0)/(gamma*v0)-p0);
    else { // Ideal Gas
        return 0;
    }
}


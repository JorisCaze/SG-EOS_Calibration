#include "calib_shock.h"
#include "tools.h"

using namespace std;

void readShockInput(double &c0, double &v0, double &p0, double &a, double &gaMin, double &gaMax)
{
    // Purpose : read shock.txt input file for shock calibration
    ifstream strmRefStates("input/Shock/Calib_shock.txt");
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

double seekGamma(double gaMin, double gaMax, double c0, vector<double> &u, vector<double> &dEstimated)
{
    // Purpose : find the best adiabatic index gamma from a range of gamma data with with Ordinary Lest Squares (OLS) procedure and plot the curve D=f(u,gamma_i)
    // More : at each gamma_i the shock speed D=f(u,gamma_i) is evaluated and the residual is computed with the linearized experimental curve
    // More : write formatted file with two columns for each gamma
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
            ga += 0.1;
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
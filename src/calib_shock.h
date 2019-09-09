#ifndef CALIB_SHOCH_H
#define CALIB_SHOCK_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

void readShockInput(double &c0, double &v0, double &p0, double &a, double &gaMin, double &gaMax);
// double determineAdiabCoeff(std::vector<double> const &tab_x, std::vector<double> const &tab_y);
void writeExperimentalPlotFile(std::vector<double> &u, std::vector<double> &dEstimated);
double computeExperimentalShockSpeed(double c0, double a, double u);
double determineAdiabCoeff(std::vector<double> &tab_x, std::vector<double> &tab_y, double c0);
double computeTheoricShockSpeed(double c0, double gamma, double u);
double seekGamma(double gaMin, double gaMax, double c0, std::vector<double> &u, std::vector<double> &dEstimated);
double computePinfShock(double c0, double v0, double p0, double gamma);

#endif // CALIB_SHOCK_H
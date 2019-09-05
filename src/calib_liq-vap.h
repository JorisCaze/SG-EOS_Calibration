#ifndef CALIB_LIQVAP_H
#define CALIB_LIQVAP_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

// Reading experimental data files
void readRefStates(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bis, double &vL0, double &vG0, double &pSat0, double &T1bis, double &vL1, double &vG1, double &pSat1, double &qPrimG);
void readDoubleFile(std::string const &file1, std::string const &file2, std::vector<double> &tab1, std::vector<double> &T_tab1, std::vector<double> &tab2, std::vector<double> &T_tab2);

double computeCpk(double hk0, double T0, double hk1, double T1);
double computeQk(double hk0, double T0, double cpk);
double computePinfK(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1);
double computeCvk(double cpk, double vk0, double T0, double pSat0, double pInfK);
double computeGammaK(double cpk, double cvk);

// double fnPstar(double p, double T, double qPrimG, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG);


#endif // CALIB_LIQVAP_H
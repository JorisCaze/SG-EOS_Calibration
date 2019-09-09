#ifndef CALIB_LIQVAP_H
#define CALIB_LIQVAP_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

// Reading experimental data files
void readLiqVapInput(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bisL, double &vL0, double &pSat0L, double &T0bisG, double &vG0, double &pSat0G, double &T1bisL, double &vL1, double &pSat1L, double &T1bisG, double &vG1, double &pSat1G);

// Differential Method (DM) with two reference points
double computeCpkDM(double hk0, double T0, double hk1, double T1);
double computePinfkDM(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1);
double computeCvkDM(double cpk, double vk0, double T0, double pSat0, double pInfK);

// Least Squares Method with experimental data
double computeCpkLSM(std::vector<double> hkExp, std::vector<double> Texp);
// Info : we can use the same step as [2]
// double computeCvkLSM(std::vector<double> vkExp, std::vector<double> Texp, std::vector<double> Pexp, double cpk);
// double computePinfkLSM(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1);

// Compatible with both methods
double computeQk(double hk, double T, double cpk);
double computeGammak(double cpk, double cvk);
double computeQprimG(std::vector<double> p, std::vector<double> T, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG);


#endif // CALIB_LIQVAP_H
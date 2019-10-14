#ifndef CALIB_LIQVAP_H
#define CALIB_LIQVAP_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

// Reading experimental data files
void readLiqVapRefStatesDM(double &T0, double &hL0, double &hG0, double &T1, double &hL1, double &hG1, double &T0bisL, double &vL0, double &pSat0L, double &T0bisG, double &vG0, double &pSat0G, double &T1bisL, double &vL1, double &pSat1L, double &T1bisG, double &vG1, double &pSat1G);
void readRefStateLSM(double &p0, double &ro0, double &c0, double &tpStart, double &tpEnd);
void readExpDataLSM(std::string file, std::vector<double> &Texp, std::vector<double> &PsatExp, std::vector<double> &vGexp, std::vector<double> &vLexp, std::vector<double> &hGexp, std::vector<double> &hLexp, std::vector<double> &LvExp);

// Differential Method (DM) with two reference points
double computeCpkDM(double hk0, double T0, double hk1, double T1);
double computePinfkDM(double vk0, double vk1, double T0, double T1, double pSat0, double pSat1);
double computeCvkDM(double cpk, double vk0, double T0, double pSat0, double pInfK);

// Least Squares Method with experimental data
double computeCpkLSM(std::vector<double> const &hkExp, std::vector<double> const &Texp, int istart = 0, int iend = -1);
double computeCvgLSM(std::vector<double> const &Texp, std::vector<double> const &PsatExp, std::vector<double> const &vGexp, double cpG);
double computeHeatCapDiffkLSM(std::vector<double> const& PsatExp, std::vector<double> const& Texp, std::vector<double> const& vKexp, double pinfk);
double computeDHeatCapDiffkLSM(std::vector<double> const& PsatExp, std::vector<double> const& Texp, std::vector<double> const& vKexp, double pinfk);
double computePinfkLSM(std::vector<double> const &PsatExp, std::vector<double> const &Texp, std::vector<double> const &vKexp, double cp, double p0, double ro0, double c0);

// Compatible with both methods
double computeQk(double hk, double T, double cpk);
double computeGammak(double cpk, double cvk);
double computeQprimG(std::vector<double> const &p, std::vector<double> const& T, double cpL, double cpG, double cvL, double cvG, double qL, double qG, double pInfL, double pInfG);

// Theoric curves
void coeffPsatTh(double cpG, double cpL, double cvG, double cvL, double qG, double qL, double qPrimG, double &A, double &B, double &C, double &D);
double computeThEnthalpy(double cpk, double qk, double T);
double computePsatTh(double A, double B, double C, double D, double pinfG, double pinfL, double T);
double computeVkTh(double cpk, double cvk, double pinfk, double T, double Psat);


#endif // CALIB_LIQVAP_H
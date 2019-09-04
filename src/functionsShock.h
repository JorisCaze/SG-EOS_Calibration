#ifndef FUNCTIONSREF_H
#define FUNCTIONSREF_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

void readFile(std::string const &file1, std::vector<double> &tab_x, std::vector<double> &tab_y);
double computeTheoricShockSpeed(double c0, double gamma, double u);
double computePinf(double c0, double v0, double p0, double gamma);


#endif;
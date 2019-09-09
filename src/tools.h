#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

void readFile(std::string const &file, std::vector<double> &tab_x, std::vector<double> &tab_y);

std::string toStrShort(double number, int const shr = 5);

// Least Square Method tools
double meanValue(std::vector<double> &tab);
double residual(std::vector<double> &tabKnow, std::vector<double> &tabEstimated);

#endif // TOOLS_H
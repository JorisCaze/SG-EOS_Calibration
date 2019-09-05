#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <vector>

std::string toStrShort(double number, int const shr = 5);
double meanValue(std::vector<double> &tab);
double residual(std::vector<double> &tabKnow, std::vector<double> &tabEstimated);

#endif // TOOLS_H
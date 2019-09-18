#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

void displayHeader();
void readFile(std::string const &file, std::vector<double> &tab_x, std::vector<double> &tab_y);
void mvFileToRes(std::string dir, std::string file);
void writePlotFile(std::string file, std::vector<double> &x, std::vector<double> &y);

std::string toStrShort(double number, int const shr = 5);

// Least Square Method tools
double meanValue(std::vector<double> &tab);
double meanValue(std::vector<double> const &tab);
double residual(std::vector<double> &tabKnow, std::vector<double> &tabEstimated);

#endif // TOOLS_H
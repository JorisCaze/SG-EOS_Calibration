#include "tools.h"

using namespace std;

void displayHeader()
{
    cout << R"(
     _____ _____   _____       _ _ _               _   _             
    /  ___|  __ \ /  __ \     | (_) |             | | (_)            
    \ `--.| |  \/ | /  \/ __ _| |_| |__  _ __ __ _| |_ _  ___  _ __  
     `--. \ | __  | |    / _` | | | '_ \| '__/ _` | __| |/ _ \| '_ \ 
    /\__/ / |_\ \ | \__/\ (_| | | | |_) | | | (_| | |_| | (_) | | | |
    \____/ \____/  \____/\__,_|_|_|_.__/|_|  \__,_|\__|_|\___/|_| |_|
    )" << '\n';
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

void mvFileToRes(string dir, string file)
{
    ifstream strmIn(dir+file);
    string fileOut("res/"+file);
    ofstream strmOut(fileOut.c_str()); 
    string line;
    double data1(0.), data2(0.);
    if (strmIn) {
        getline(strmIn,line);
        while (getline(strmIn,line)) {
            strmOut << line << endl;
        }
    } 
    else {
        cout << "Warning : moving experimental data file " << file << ".txt\n";
    }
}

void writePlotFile(string file, vector<double> &x, vector<double> &y)
{
    // Purpose : write file with two columns 
    ofstream strm(file.c_str());
    for (unsigned int i = 0; i < x.size(); i++) {
        strm << x[i] << " " << y[i] << endl;
    }
}

string toStrShort(double number, int const shr)
{
    // Purpose : convert a string to double and shorten his length 
    // More : by default it will shorten the given string by 5 characters
    string wordShort(to_string(number));
    for (int i=0; i<shr; i++) 
        wordShort.pop_back();
    return wordShort;
}

double meanValue(vector<double> &tab)
{
    // Purpose : compute the mean value of a vector
    double buf(0.);
    for (unsigned int i = 0; i < tab.size(); i++) {
        buf += tab[i];
    }
    return (buf/tab.size());
}

double meanValue(vector<double> const &tab)
{
    // Purpose : compute the mean value of a vector
    double buf(0.);
    for (unsigned int i = 0; i < tab.size(); i++) {
        buf += tab[i];
    }
    return (buf/tab.size());
}

double residual(std::vector<double> &tabKnow, std::vector<double> &tabEstimated)
{
    // Purpose : compute the residual of an Ordinary Lest Squares (OLS) procedure
    // Linear fn y=f(x) - vector are y values
    double scr(0.), sct(0.);
    if (tabKnow.size() == tabEstimated.size()) {
        for (unsigned int i = 0; i < tabKnow.size(); i++) {
        scr += (tabKnow[i]-tabEstimated[i])*(tabKnow[i]-tabEstimated[i]);
        sct += (tabKnow[i]-meanValue(tabKnow))*(tabKnow[i]-meanValue(tabKnow)); 
        }
    }
    else {
        cout << "Error : residual can not be computed due to different vector size\n";
    }
    return (1.-(scr/sct));
}
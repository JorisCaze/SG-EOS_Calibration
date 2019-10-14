#include "tools.h"

using namespace std;

void displayHeader()
{
    cout << "   _____ _____   _____       _ _ _               _   _             " << endl;
    cout << "  /  ___|  __ \\ /  __ \\     | (_) |             | | (_)            " << endl;
    cout << "  \\ `--.| |  \\/ | /  \\/ __ _| |_| |__  _ __ __ _| |_ _  ___  _ __  " << endl;
    cout << "   `--. \\ | __  | |    / _` | | | '_ \\| '__/ _` | __| |/ _ \\| '_ \\ " << endl;
    cout << "  /\\__/ / |_\\ \\ | \\__/\\ (_| | | | |_) | | | (_| | |_| | (_) | | | |" << endl;
    cout << "  \\____/ \\____/  \\____/\\__,_|_|_|_.__/|_|  \\__,_|\\__|_|\\___/|_| |_|" << endl;
    cout << endl;
}

void readInput(int &calibration, int &liVapMethod)
{
    // Purpose : read input file (selection of calibration and method)
    ifstream strmIn("input/input.txt");
    string line;
    if (strmIn){
        for (int i = 1; i < 6; i++){getline(strmIn,line);}
        calibration = stoi(line);
        for (int i = 1; i < 7; i++){getline(strmIn,line);}    
        liVapMethod = stoi(line);
    }
    else {
        cout << "Error : reading input.txt file\n"; exit(0); 
    }
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

double meanValue(vector<double> &tab, int istart, int iend)
{
    // Purpose : compute the mean value of a vector
    double buf(0.);
    if (iend == -1) iend = tab.size();
    for (int i = istart; i < iend; i++) {
        buf += tab[i];
    }
    return (buf/(iend-istart));
}

double meanValue(vector<double> const &tab, int istart, int iend)
{
    // Purpose : compute the mean value of a vector
    double buf(0.);
    if (iend == -1) iend = tab.size();
    for (int i = istart; i < iend; i++) {
        buf += tab[i];
    }
    return (buf/(iend-istart));
}

double residual(std::vector<double> &tabKnow, std::vector<double> &tabEstimated)
{
    // Purpose : compute the residual of an Ordinary Lest Squares (OLS) procedure
    // Linear fn y=f(x) - vector are y values
    double scr(0.), sct(0.), meanTabKnow(0.);
    meanTabKnow = meanValue(tabKnow);
    if (tabKnow.size() == tabEstimated.size()) {
        for (unsigned int i = 0; i < tabKnow.size(); i++) {
            scr += (tabKnow[i]-tabEstimated[i])*(tabKnow[i]-tabEstimated[i]);
            sct += (tabKnow[i]-meanTabKnow)*(tabKnow[i]-meanTabKnow); 
        }
    }
    else {
        cout << "Error : residual can not be computed due to different vector size\n";
    }
    return (1.-(scr/sct));
}

void writeResults(double cpL, double qL, double pinfL, double cvL, double gammaL, double qPrimL, double cpG, double qG, double pinfG, double cvG, double gammaG, double qPrimG)
{
    ofstream strmRes("res/res.txt");
    strmRes << "* Results *\n";
    strmRes << "--- Liquid (L) ---\n";
    strmRes << "cpL    (J.kg-1.K-1)  : " << cpL << endl;
    strmRes << "qL     (J.kg-1)      : " << qL << endl;
    strmRes << "pinfL  (Pa)          : " << pinfL << endl;
    strmRes << "cvL    (J.kg-1.K-1)  : " << cvL << endl;
    strmRes << "gammaL (-)           : " << gammaL << endl;
    strmRes << "q'L    (J.kg-1)      : " << qPrimL << endl;
    strmRes << "\n";

    strmRes << "--- Gas (G) ---\n";
    strmRes << "cpG    (J.kg-1.K-1)  : " << cpG << endl;
    strmRes << "qG     (J.kg-1)      : " << qG << endl;
    strmRes << "pinfG  (Pa)          : " << pinfG << endl;
    strmRes << "cvG    (J.kg-1.K-1)  : " << cvG << endl;
    strmRes << "gammaG (-)           : " << gammaG << endl;
    strmRes << "q'G    (J.kg-1)      : " << qPrimG << endl;
} 
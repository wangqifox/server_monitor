#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<string> getWords(string line);
string runCommand(const char* cmd);
double doubleSetPrecision(double num, int precision);
string doubleTostring(double num, int precision);
int getPageSize();

template<typename T>
string tostring(T t){
    stringstream ss;
    ss << t;
    return ss.str();
}

#endif

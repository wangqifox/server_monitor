#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
/*#include "cpu.hpp"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"*/

using namespace std;

vector<string> getWords(string line);
string runCommand(const char* cmd);
double doubleSetPrecision(double num, int precision);
string doubleTostring(double num, int precision);

template<typename T>
string tostring(T t){
    stringstream ss;
    ss << t;
    return ss.str();
}

int getPageSize();

#endif

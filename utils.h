#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"

using namespace std;

vector<string> getWords(string line);
string runCommand(const char* cmd);

#endif

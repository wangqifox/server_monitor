#ifndef MEMINFO_H
#define MEMINFO_H

#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

class MemInfo {
public:
    time_t time;
    unsigned long long memTotal;
    unsigned long long memFree;
    unsigned long long buffers;
    unsigned long long cached;
    unsigned long long swapTotal;
    unsigned long long swapFree;

    MemInfo():time(0), memTotal(0), memFree(0), buffers(0), cached(0), swapTotal(0), swapFree(0){}

    friend ostream& operator<< (ostream &os, MemInfo &meminfo);
};

#endif
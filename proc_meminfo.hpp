#ifndef MEMINFO_HPP
#define MEMINFO_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

const string MEMFILE = "/proc/meminfo";

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

    friend ostream& operator<< (ostream &os, MemInfo &meminfo) {
        os << "time: " << meminfo.time << " "
            << "memTotal: " << meminfo.memTotal / 1024.0 << "M " 
            << "memFree: " << meminfo.memFree / 1024.0 << "M " 
            << "buffers: " << meminfo.buffers / 1024.0 << "M " 
            << "cached: " << meminfo.cached / 1024.0 << "M " 
            << "swapTotal: " << meminfo.swapTotal / 1024.0 << "M " 
            << "swapFree: " << meminfo.swapFree / 1024.0 << "M";
            
        return os;
    }
};

namespace{
    MemInfo readMemInfo() {
        ifstream in(MEMFILE);
        string line;
        MemInfo meminfo;
        meminfo.time = time(NULL);
        if(in) {
            while(getline(in, line)) {
                vector<string> words = getWords(line);
                if(words[0].find("MemTotal") == 0) {
                    meminfo.memTotal = stoull(words[1]);
                } else if(words[0].find("MemFree") == 0) {
                    meminfo.memFree = stoull(words[1]);
                } else if(words[0].find("Buffers") == 0) {
                    meminfo.buffers = stoull(words[1]);
                } else if(words[0].find("Cached") == 0) {
                    meminfo.cached = stoull(words[1]);
                } else if(words[0].find("SwapTotal") == 0) {
                    meminfo.swapTotal = stoull(words[1]);
                } else if(words[0].find("SwapFree") == 0) {
                    meminfo.swapFree = stoull(words[1]);
                }
            }
        }
        in.close();
        return meminfo;
    }
}
#endif
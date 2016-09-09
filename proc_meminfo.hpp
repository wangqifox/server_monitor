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
    time_t time_stamp;
    unsigned long long memTotal;
    unsigned long long memFree;
    unsigned long long buffers;
    unsigned long long cached;
    unsigned long long swapTotal;
    unsigned long long swapFree;

    MemInfo():time_stamp(0), memTotal(0), memFree(0), buffers(0), cached(0), swapTotal(0), swapFree(0){}

    friend ostream& operator<< (ostream &os, MemInfo &meminfo) {
        os << "time: " << meminfo.time_stamp << " "
            << "memTotal: " << meminfo.memTotal / 1024.0 << "M " 
            << "memFree: " << meminfo.memFree / 1024.0 << "M " 
            << "buffers: " << meminfo.buffers / 1024.0 << "M " 
            << "cached: " << meminfo.cached / 1024.0 << "M " 
            << "swapTotal: " << meminfo.swapTotal / 1024.0 << "M " 
            << "swapFree: " << meminfo.swapFree / 1024.0 << "M";
            
        return os;
    }

    void readMemInfo() {
        ifstream in(MEMFILE);
        string line;
        time_stamp = time(NULL);
        if(in) {
            while(getline(in, line)) {
                vector<string> words = getWords(line);
                if(words[0].find("MemTotal") == 0) {
                    memTotal = stoull(words[1]);
                } else if(words[0].find("MemFree") == 0) {
                    memFree = stoull(words[1]);
                } else if(words[0].find("Buffers") == 0) {
                    buffers = stoull(words[1]);
                } else if(words[0].find("Cached") == 0) {
                    cached = stoull(words[1]);
                } else if(words[0].find("SwapTotal") == 0) {
                    swapTotal = stoull(words[1]);
                } else if(words[0].find("SwapFree") == 0) {
                    swapFree = stoull(words[1]);
                }
            }
        }
        in.close();
    }
};

#endif
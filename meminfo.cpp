#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "meminfo.h"
using namespace std;

ostream& operator<< (ostream &os, MemInfo &meminfo) {
    os << "time: " << meminfo.time << " "
        << "memTotal: " << meminfo.memTotal / 1024.0 << "M " 
        << "memFree: " << meminfo.memFree / 1024.0 << "M " 
        << "buffers: " << meminfo.buffers / 1024.0 << "M " 
        << "cached: " << meminfo.cached / 1024.0 << "M " 
        << "swapTotal: " << meminfo.swapTotal / 1024.0 << "M " 
        << "swapFree: " << meminfo.swapFree / 1024.0 << "M";
        
    return os;
}
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

const string MEMFILE = "/proc/meminfo";

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

int main_meminfo() {
    MemInfo meminfo = readMemInfo();
    cout << meminfo << endl;
    return 1;
}
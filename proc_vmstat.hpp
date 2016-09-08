#ifndef VMSTAT_H
#define VMSTAT_H

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

const string VMSTATFILE = "/proc/vmstat";

class Vmstat {
public:
    time_t time;
    unsigned long long pgpgin;
    unsigned long long pgpgout;
    unsigned long long pswpin;
    unsigned long long pswpout;

    Vmstat():time(0), pgpgin(0),pgpgout(0),pswpin(0),pswpout(0){}

    friend ostream& operator<< (ostream &os, Vmstat &vmstat) {
        os << "time: " << vmstat.time << " "
            << "pgpgin: " << vmstat.pgpgin << " "
            << "pgpgout: " << vmstat.pgpgout << " " 
            << "pswpin: " << vmstat.pswpin << " "
            << "pswpout: " << vmstat.pswpout;
        return os;
    }
    friend Vmstat operator- (const Vmstat &vmstat1, const Vmstat &vmstat2) {
        Vmstat vmstat;
        vmstat.pgpgin = vmstat1.pgpgin - vmstat2.pgpgin;
        vmstat.pgpgout = vmstat1.pgpgout - vmstat2.pgpgout;
        vmstat.pswpin = vmstat1.pswpin - vmstat2.pswpin;
        vmstat.pswpout = vmstat1.pswpout - vmstat2.pswpout;
        return vmstat;
    }
};

namespace{
    Vmstat readVmstat() {
        ifstream in(VMSTATFILE);
        string line;
        Vmstat vmstat;
        vmstat.time = time(NULL);
        if(in){
            while(getline(in, line)) {
                vector<string> words = getWords(line);
                if(words[0] == "pgpgin"){
                    vmstat.pgpgin = stoull(words[1]);
                } else if(words[0] == "pgpgout"){
                    vmstat.pgpgout = stoull(words[1]);
                } else if(words[0] == "pswpin"){
                    vmstat.pswpin = stoull(words[1]);
                } else if(words[0] == "pswpout"){
                    vmstat.pswpout = stoull(words[1]);
                }
            }
        }
        in.close();
        return vmstat;
    }
}
#endif
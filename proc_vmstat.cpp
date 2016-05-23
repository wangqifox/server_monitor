#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

const string VMSTATFILE = "/proc/vmstat";

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

int main_vmstat() {
    time_t t = time(NULL);
    cout << "current time is " << t << endl;
    Vmstat vmstat1 = readVmstat();
    sleep(1);
    Vmstat vmstat2 = readVmstat();

    cout << vmstat1 << endl;
    cout << vmstat2 << endl;

    Vmstat vmstatDelta = vmstat2 - vmstat1;
    cout << vmstatDelta << endl;

    string PAGE_SIZE = runCommand("getconf PAGE_SIZE");
    cout << stoul(PAGE_SIZE) << endl;

    return 0;
}
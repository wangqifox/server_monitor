#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

const string CPUFILE = "/proc/stat";

vector<string> readFile(string filename){
    ifstream in("/proc/stat");
    string line;
    vector<string> words;
    if(in){
        while(getline(in, line)){
            cout << line << endl;
            words = getWords(line);
            if(words[0] == "cpu"){
                cout << words.size() << endl;
                in.close();
                return words;
            }
        }
    }
    in.close();
    return words;
}

Cpu readCpuStat(){
    ifstream in(CPUFILE);
    string line;
    Cpu cpu;
    cpu.time = time(NULL);
    if(in){
        while(getline(in, line)){
            // cout << line << endl;
            vector<string> words = getWords(line);
            if(words[0].find("cpu") == 0){
                CpuTime cpuTime;
                cpuTime.id = words[0];
                cpuTime.user = stoull(words[1]);
                cpuTime.nice = stoull(words[2]);
                cpuTime.system = stoull(words[3]);
                cpuTime.idle = stoull(words[4]);
                cpuTime.iowait = stoull(words[5]);
                cpuTime.irq = stoull(words[6]);
                cpuTime.softirq = stoull(words[7]);
                cpuTime.steal = stoull(words[8]);

                if(words.size() >= 10) cpuTime.guest = stoull(words[9]);
                if(words.size() >= 11) cpuTime.guest_nice = stoull(words[10]);

                cpu.cpu.push_back(cpuTime);
            } else if(words[0] == "ctxt") {
                cpu.ctxt = stoull(words[1]);
            } else if(words[0] == "processes") {
                cpu.processes = stoull(words[1]);
            } else if(words[0] == "procs_running") {
                cpu.procs_running = stoull(words[1]);
            } else if(words[0] == "procs_blocked") {
                cpu.procs_blocked = stoull(words[1]);
            }
        }
    }
    in.close();
    return cpu;
}

int main_stat() {
    time_t t = time(NULL);
    cout << "current time is " << t << endl;
    Cpu cpu1 = readCpuStat();
    sleep(1);
    Cpu cpu2 = readCpuStat();

    cout << cpu1 << endl;
    cout << cpu2 << endl;

    vector<CpuRate> cpuRateVec = cpuRate(cpu1, cpu2);
    for(CpuRate cpuRate : cpuRateVec){
        cout << cpuRate << endl;
    }
    
    return 1;
}

int main2() {
    time_t t = time(NULL);
    cout << "current time is " << t << endl;
    string filename = "/proc/stat";
    cout << filename << endl;
    vector<string> data1 = readFile(filename);
    sleep(1);
    t = time(NULL);
    cout << "current time is " << t << endl;
    vector<string> data2 = readFile(filename);
    vector<long> delta_vec;
    for(size_t i = 1 ; i < data1.size() ; i++) {
        long delta = stol(data2[i]) - stol(data1[i]);
        // cout << delta << endl;
        delta_vec.push_back(delta);
    }

    long int sum = 0;
    for(size_t i = 0 ; i < delta_vec.size() ; i++) {
        sum += delta_vec[i];
    }

    cout << (sum - delta_vec[3]) / (sum * 1.0) << endl;

    return 1;
}
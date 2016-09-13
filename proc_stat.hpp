#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.hpp"

using namespace std;

const string CPUFILE = "/proc/stat";

class CpuTime {
public:
    string id;
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long guest_nice;

    CpuTime():id(""), user(0), nice(0), system(0), idle(0), iowait(0), irq(0), softirq(0), steal(0), guest(0), guest_nice(0){}

    friend ostream& operator<< (ostream &os, const CpuTime& cpuTime) {
        os << cpuTime.id << " " 
            << cpuTime.user << " " 
            << cpuTime.nice << " " 
            << cpuTime.system << " " 
            << cpuTime.idle << " " 
            << cpuTime.iowait << " " 
            << cpuTime.irq << " " 
            << cpuTime.softirq << " " 
            << cpuTime.steal << " " 
            << cpuTime.guest << " " 
            << cpuTime.guest_nice;
        return os;
    }
    friend CpuTime operator- (const CpuTime &cpuTime1, const CpuTime &cpuTime2) {
        CpuTime cpuTime;
        cpuTime.user = cpuTime1.user - cpuTime2.user;
        cpuTime.nice = cpuTime1.nice - cpuTime2.nice;
        cpuTime.system = cpuTime1.system - cpuTime2.system;
        cpuTime.idle = cpuTime1.idle - cpuTime2.idle;
        cpuTime.iowait = cpuTime1.iowait - cpuTime2.iowait;
        cpuTime.irq = cpuTime1.irq - cpuTime2.irq;
        cpuTime.softirq = cpuTime1.softirq - cpuTime2.softirq;
        cpuTime.steal = cpuTime1.steal - cpuTime2.steal;
        cpuTime.guest = cpuTime1.guest - cpuTime2.guest;
        cpuTime.guest_nice = cpuTime1.guest_nice - cpuTime2.guest_nice;
        return cpuTime;
    }
};

class CpuRate {
public:
    string id;
    double total;
    double user;
    double nice;
    double system;
    double idle;
    double iowait;
    double irq;
    double softirq;
    double steal;
    double guest;
    double guest_nice;

    CpuRate():id(""), total(0.0), user(0.0), nice(0.0), system(0.0), idle(0.0), iowait(0.0), irq(0.0), softirq(0.0), steal(0.0), guest(0.0), guest_nice(0.0){}

    friend ostream& operator<< (ostream &os, CpuRate &cpuRate) {
        os << cpuRate.id << endl
            << "\t" << "total: " << cpuRate.total << endl
            << "\t" << "user: " << cpuRate.user << endl
            << "\t" << "nice: " << cpuRate.nice << endl
            << "\t" << "system: " << cpuRate.system << endl
            << "\t" << "idle: " << cpuRate.idle << endl
            << "\t" << "iowait: " << cpuRate.iowait << endl
            << "\t" << "irq: " << cpuRate.irq << endl
            << "\t" << "softirq: " << cpuRate.softirq << endl
            << "\t" << "steal: " << cpuRate.steal << endl
            << "\t" << "guest: " << cpuRate.guest << endl
            << "\t" << "guest_nice: " << cpuRate.guest_nice;
        return os;
    }

};

class Cpu {
public:
    time_t time_stamp;
    vector<CpuTime> cpu;
    unsigned long long ctxt;
    unsigned long long processes;
    unsigned long long procs_running;
    unsigned long long procs_blocked;

    friend ostream& operator<< (ostream &os, Cpu &cpu) {
        os << "time: " << cpu.time_stamp << endl
            << "cpu: " << endl;
        for(CpuTime cpuTime : cpu.cpu) {
            os << "\t" << cpuTime << endl;
        }
        os << "ctxt: " << cpu.ctxt << endl
            << "processes: " << cpu.processes << endl
            << "procs_running: " << cpu.procs_running << endl
            << "procs_blocked: " << cpu.procs_blocked << endl;

        return os;
    }

    friend vector<CpuRate> cpuRate(const Cpu &cpu1, const Cpu &cpu2) {
        vector<CpuRate> cpuRateVec;
        for(size_t i = 0; i < cpu1.cpu.size(); i++) {
            CpuRate cpuRate;
            CpuTime cpuTime1 = cpu1.cpu[i];
            CpuTime cpuTime2 = cpu2.cpu[i];
            // cout << cpuTime1 << endl;
            // cout << cpuTime2 << endl;
            CpuTime cpuTimeDelta = cpuTime2 - cpuTime1;
            // cout << cpuTimeDelta << endl;
            unsigned long long sum = cpuTimeDelta.user 
                + cpuTimeDelta.nice 
                + cpuTimeDelta.system 
                + cpuTimeDelta.idle 
                + cpuTimeDelta.iowait 
                + cpuTimeDelta.irq 
                + cpuTimeDelta.softirq 
                + cpuTimeDelta.steal
                + cpuTimeDelta.guest
                + cpuTimeDelta.guest_nice;

            cpuRate.id = cpuTime1.id;
            cpuRate.total = (sum - cpuTimeDelta.idle) / (sum * 1.0);
            cpuRate.user = (cpuTimeDelta.user) / (sum * 1.0);
            cpuRate.nice = (cpuTimeDelta.nice) / (sum * 1.0);
            cpuRate.system = (cpuTimeDelta.system) / (sum * 1.0);
            cpuRate.idle = (cpuTimeDelta.idle) / (sum * 1.0);
            cpuRate.iowait = (cpuTimeDelta.iowait) / (sum * 1.0);
            cpuRate.irq = (cpuTimeDelta.irq) / (sum * 1.0);
            cpuRate.softirq = (cpuTimeDelta.softirq) / (sum * 1.0);
            cpuRate.steal = (cpuTimeDelta.steal) / (sum * 1.0);
            cpuRate.guest = (cpuTimeDelta.guest) / (sum * 1.0);
            cpuRate.guest_nice = (cpuTimeDelta.guest_nice) / (sum * 1.0);

            cpuRateVec.push_back(cpuRate);
        }
        return cpuRateVec;
    }

    void readCpuStat() {
        ifstream in(CPUFILE);
        string line;
        time_stamp = time(NULL);
        if(in) {
            while(getline(in, line)) {
                vector<string> words = Util::getWords(line);
                if(words[0].find("cpu") == 0) {
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

                    cpu.push_back(cpuTime);
                } else if(words[0] == "ctxt") {
                    ctxt = stoull(words[1]);
                } else if(words[0] == "processes") {
                    processes = stoull(words[1]);
                } else if(words[0] == "procs_running") {
                    procs_running = stoull(words[1]);
                } else if(words[0] == "procs_blocked") {
                    procs_blocked = stoull(words[1]);
                }
            }
        }
        in.close();
    }
};

#endif
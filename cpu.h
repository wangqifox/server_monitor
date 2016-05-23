#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

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

    friend ostream& operator<< (ostream &os, const CpuTime& cpuTime);
    friend CpuTime operator- (const CpuTime &cpuTime1, const CpuTime &cpuTime2);
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

    friend ostream& operator<< (ostream &os, CpuRate &cpuRate);

};

class Cpu {
public:
    time_t time;
    vector<CpuTime> cpu;
    unsigned long long ctxt;
    unsigned long long processes;
    unsigned long long procs_running;
    unsigned long long procs_blocked;

    friend ostream& operator<< (ostream &os, Cpu &cpu);
    friend vector<CpuRate> cpuRate(const Cpu &cpu1, const Cpu &cpu2);
};

#endif
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "cpu.h"
using namespace std;

ostream& operator<< (ostream &os, const CpuTime& cpuTime) {
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

CpuTime operator- (const CpuTime &cpuTime1, const CpuTime &cpuTime2) {
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

ostream& operator<< (ostream &os, CpuRate &cpuRate) {
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

ostream& operator<< (ostream &os, Cpu &cpu) {
    os << "time: " << cpu.time << endl
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

vector<CpuRate> cpuRate(const Cpu& cpu1, const Cpu& cpu2) {
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
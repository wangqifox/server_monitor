#ifndef PROCMAIN_H
#define PROCMAIN_H

#include <queue>
#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"

using namespace std;

void proc_main();

extern queue<Cpu> cpu_queue;
extern queue<MemInfo> memInfo_queue;
extern queue<Vmstat> vmstat_queue;
extern queue<Netstat> netstat_queue;

extern Cpu readCpuStat();
extern MemInfo readMemInfo();
extern Vmstat readVmstat();
extern Netstat readNetstat();

#endif

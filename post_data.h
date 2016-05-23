#ifndef POSTDATA_H
#define POSTDATA_H

#include <queue>
#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"

using namespace std;

extern queue<Cpu> cpu_queue;
extern queue<MemInfo> memInfo_queue;
extern queue<Vmstat> vmstat_queue;
extern queue<Netstat> netstat_queue;

void post_data();

#endif

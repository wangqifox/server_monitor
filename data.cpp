#include <queue>

#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"

using namespace std;

queue<Cpu> cpu_queue;
queue<MemInfo> memInfo_queue;
queue<Vmstat> vmstat_queue;
queue<Netstat> netstat_queue;

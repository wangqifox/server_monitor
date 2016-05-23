#include <iostream>
#include <time.h>
#include <unistd.h>
#include "proc_main.h"

void proc_main() {
    while(true) {
        Cpu cpu = readCpuStat();
        MemInfo meminfo = readMemInfo();
        Vmstat vmstat = readVmstat();
        Netstat netstat = readNetstat();

        cpu_queue.push(cpu);
        memInfo_queue.push(meminfo);
        vmstat_queue.push(vmstat);
        netstat_queue.push(netstat);
        
        sleep(1);
    }

}
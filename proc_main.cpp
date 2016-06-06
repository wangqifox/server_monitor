#include <iostream>
#include <time.h>
#include <unistd.h>
#include "proc_main.h"

void proc_main(ServerData* serverData) {
    while(true) {
        cout << "proc_main" << endl;
        Cpu cpu = readCpuStat();
        MemInfo meminfo = readMemInfo();
        Vmstat vmstat = readVmstat();
        Netstat netstat = readNetstat();
        
        serverData->add_cpu(cpu);
        serverData->add_meminfo(meminfo);
        serverData->add_vmstat(vmstat);
        serverData->add_netstat(netstat);

        sleep(1);
    }

}
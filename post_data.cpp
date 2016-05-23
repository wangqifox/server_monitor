#include <iostream>
#include <time.h>
#include <unistd.h>
#include "post_data.h"
#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"

using namespace std;

void post_cpu() {
    if(!cpu_queue.empty()){
        Cpu cpu = cpu_queue.front();
        cout << cpu << endl;
        cpu_queue.pop();
    }
}

void post_mem() {
    if(!memInfo_queue.empty()){
        MemInfo meminfo = memInfo_queue.front();
        cout << meminfo << endl;
        memInfo_queue.pop();
    }
}

void post_disk() {
    if(!vmstat_queue.empty()){
        Vmstat vmstat = vmstat_queue.front();
        cout << vmstat << endl;
        vmstat_queue.pop();
    }
}

void post_net() {
    if(!netstat_queue.empty()){
        Netstat netstat = netstat_queue.front();
        cout << netstat << endl;
        netstat_queue.pop();
    }
}

void post_data() {
    while(true) {
        post_cpu();
        post_mem();
        post_disk();
        post_net();
        sleep(1);
    }
}


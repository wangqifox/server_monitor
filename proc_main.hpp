#ifndef PROC_MAIN_HPP
#define PROC_MAIN_HPP

#include "proc_stat.hpp"
#include "proc_meminfo.hpp"
#include "proc_vmstat.hpp"
#include "proc_netstat.hpp"

#include "perf_data.hpp"

void start_proc(PerfData* perf_data, int delay) {
    while(true) {
        Cpu cpu;
        MemInfo meminfo;
        Vmstat vmstat;
        Netstat netstat;

        cpu.readCpuStat();
        meminfo.readMemInfo();
        vmstat.readVmstat();
        netstat.readNetstat();

        perf_data->cpu_buffer.add(cpu);
        perf_data->memInfo_buffer.add(meminfo);
        perf_data->vmstat_buffer.add(vmstat);
        perf_data->netstat_buffer.add(netstat);

        sleep(delay);
    }
}

#endif
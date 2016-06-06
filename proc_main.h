#ifndef PROCMAIN_H
#define PROCMAIN_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"
#include "post_data.h"

using namespace std;

void proc_main(ServerData* serverData);

extern Cpu readCpuStat();
extern MemInfo readMemInfo();
extern Vmstat readVmstat();
extern Netstat readNetstat();

#endif

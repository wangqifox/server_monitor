#ifndef POST_DATA_HPP
#define POST_DATA_HPP

#include "proc_stat.hpp"
#include "proc_meminfo.hpp"
#include "proc_vmstat.hpp"
#include "proc_netstat.hpp"

#include "websocket_server.h"
#include "perf_data.hpp"

class PostData {
private:
    WebsocketServer* websocket_server;
    PerfData* perf_data;

    Cpu* cpu_before;
    Vmstat* vmstat_before;
    Netstat* netstat_before;

public:
    PostData(WebsocketServer* websocket_server, PerfData* perf_data):websocket_server(websocket_server), perf_data(perf_data) {}

    virtual ~PostData() {
        delete cpu_before;
        delete vmstat_before;
        delete netstat_before;
    }

    void post(string data);

    void post_cpu();
    void post_meminfo();
    void post_vmstat();
    void post_netstat();
    void post_traffic();

    void post_cpu_task();
    void post_meminfo_task();
    void post_vmstat_task();
    void post_netstat_task();
    void post_traffic_task();

    void start();
};

#endif
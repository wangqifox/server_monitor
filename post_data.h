#ifndef POSTDATA_H
#define POSTDATA_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>

#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"
#include "utils.h"
#include "json/json.h"

using websocketpp::connection_hdl;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

class ServerData{
private:
    Cpu* cpu_before;
    Vmstat* vmstat_before;
    Netstat* netstat_before;
    int page_size;

    queue<Cpu> cpu_queue;
    queue<MemInfo> memInfo_queue;
    queue<Vmstat> vmstat_queue;
    queue<Netstat> netstat_queue;

    mutex cpu_queue_mtx;
    mutex memInfo_queue_mtx;
    mutex vmstat_queue_mtx;
    mutex netstat_queue_mtx;

    condition_variable cpu_queue_not_empty;
    condition_variable memInfo_queue_not_empty;
    condition_variable vmstat_queue_not_empty;
    condition_variable netstat_queue_not_empty;

    struct url_data {
        size_t size;
        char *data;
    };

    server* m_server;
    con_list* m_connections;

    static size_t write_data(void *ptr, size_t size, size_t nmemb, url_data *data);
    void post(string data);

public:
    ServerData(server* server, con_list* connections):cpu_before(NULL),vmstat_before(NULL),netstat_before(NULL),m_server(server),m_connections(connections){
        page_size = getPageSize();
    }

    virtual ~ServerData(){
        delete cpu_before;
        delete vmstat_before;
        delete netstat_before;
    }

    void add_cpu(Cpu& cpu);
    void add_meminfo(MemInfo& meminfo);
    void add_vmstat(Vmstat& vmstat);
    void add_netstat(Netstat& netstat);

    void post_cpu();
    void post_meminfo();
    void post_vmstat();
    void post_netstat();
};

void post_data(ServerData* serverData);

#endif

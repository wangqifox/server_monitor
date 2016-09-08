#ifndef POSTDATA_H
#define POSTDATA_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>

#include "proc_stat.hpp"
#include "proc_meminfo.hpp"
#include "proc_vmstat.hpp"
#include "proc_netstat.hpp"

#include "utils.h"
#include "json/json.h"
#include "sniffer.hpp"

using websocketpp::connection_hdl;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

template <class T>
class Repository{
private:
    static const int buffer_size = 10;
    T buffer[buffer_size];
    int read_position = 0;
    int write_position = 0;
    mutex mtx;
    condition_variable buffer_not_full;
    condition_variable buffer_not_empty;
public:
    void add(T t){
        unique_lock<mutex> lock(mtx);
        while(((write_position + 1) % buffer_size) == read_position) {
            buffer_not_full.wait(lock);
        }
        buffer[write_position] = t;
        write_position = (write_position + 1) % buffer_size;
        buffer_not_empty.notify_all();
        lock.unlock();
    }

    T fetch() {
        unique_lock<mutex> lock(mtx);
        while(write_position == read_position) {
            buffer_not_empty.wait(lock);
        }
        T t = buffer[read_position];
        read_position = (read_position + 1) % buffer_size;
        buffer_not_full.notify_all();
        lock.unlock();
        return t;
    }
};

class ServerData{
private:
    Cpu* cpu_before;
    Vmstat* vmstat_before;
    Netstat* netstat_before;
    int page_size;

    Repository<Cpu> cpu_repository;
    Repository<MemInfo> memInfo_repository;
    Repository<Vmstat> vmstat_repository;
    Repository<Netstat> netstat_repository;

    server* m_server;
    con_list* m_connections;

    void post(string data);

public:
    TrafficData trafficData = TrafficData(100);

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
    void post_traffic();

};

void post_data(ServerData* serverData, int delay=1);

#endif

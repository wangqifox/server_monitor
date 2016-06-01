#ifndef POSTDATA_H
#define POSTDATA_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"
#include "utils.h"
#include "json/json.h"

using namespace std;

// extern queue<Cpu> cpu_queue;
// extern queue<MemInfo> memInfo_queue;
// extern queue<Vmstat> vmstat_queue;
// extern queue<Netstat> netstat_queue;

// extern mutex mtx;
// extern condition_variable cpu_queue_not_empty;
// extern condition_variable memInfo_queue_not_empty;
// extern condition_variable vmstat_queue_not_empty;
// extern condition_variable netstat_queue_not_empty;



class ServerData{
private:
    Cpu* cpu_before;
    Vmstat* vmstat_before;
    Netstat* netstat_before;
    int page_size;
    string project_key;
    string post_url;
    // bool post;

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

    static size_t write_data(void *ptr, size_t size, size_t nmemb, url_data *data);
    void post(const char * url, string data);

public:
    ServerData(string url, string key):cpu_before(NULL),vmstat_before(NULL),netstat_before(NULL){
        page_size = getPageSize();
        project_key = key;
        post_url = url;
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


    // Json::Value* cpu_info();
    // Json::Value* mem_info();
    // Json::Value* disk_info();
    // Json::Value* net_info();
};

void post_data(ServerData* serverData);

#endif

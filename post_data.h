#ifndef POSTDATA_H
#define POSTDATA_H

#include <queue>
#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"
#include "utils.h"
#include "json/json.h"

using namespace std;

extern queue<Cpu> cpu_queue;
extern queue<MemInfo> memInfo_queue;
extern queue<Vmstat> vmstat_queue;
extern queue<Netstat> netstat_queue;

void post_data();

class ServerData{
private:
    Cpu* cpu_before;
    Vmstat* vmstat_before;
    Netstat* netstat_before;
    int page_size;
    string project_key;
    string post_url;
public:
    ServerData():cpu_before(NULL),vmstat_before(NULL),netstat_before(NULL){
        page_size = getPageSize();
        project_key = "tars";
        post_url = "http://tars.nie.netease.com:8282/api/perfs";
    }
    virtual ~ServerData(){
        delete cpu_before;
        delete vmstat_before;
        delete netstat_before;
    }
    Json::Value* cpu_info();
    Json::Value* mem_info();
    Json::Value* disk_info();
    Json::Value* net_info();
};

#endif

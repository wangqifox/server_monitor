#include "post_data.h"
#include "utils.hpp"
#include "json/json.h"
using namespace std;

void PostData::post(string data) {
    websocket_server->send_data(data);
}


void PostData::post_cpu() {
    Json::Value* root = NULL;
    Cpu cpu = perf_data->cpu_buffer.fetch();

    if(cpu_before == NULL) {
        cpu_before = new Cpu(cpu);
        return;
    } 

    vector<CpuRate> cpuRateVec = cpuRate(*cpu_before, cpu);

    root = new Json::Value();
    (*root)["type"] = "cpu";
    (*root)["time"] = tostring(cpu.time_stamp);
    for(CpuRate cpuRate : cpuRateVec){
        Json::Value cpu;
        cpu["id"] = cpuRate.id;
        cpu["total"] = Util::doubleTostring(cpuRate.total*100, 2);
        cpu["user"] = Util::doubleTostring(cpuRate.user*100, 2);
        cpu["nice"] = Util::doubleTostring(cpuRate.nice*100, 2);
        cpu["system"] = Util::doubleTostring(cpuRate.system*100, 2);
        cpu["idle"] = Util::doubleTostring(cpuRate.idle*100, 2);
        cpu["iowait"] = Util::doubleTostring(cpuRate.iowait*100, 2);
        cpu["irq"] = Util::doubleTostring(cpuRate.irq*100, 2);
        cpu["softirq"] = Util::doubleTostring(cpuRate.softirq*100, 2);
        cpu["steal"] = Util::doubleTostring(cpuRate.steal*100, 2);
        cpu["guest"] = Util::doubleTostring(cpuRate.guest*100, 2);
        cpu["guest_nice"] = Util::doubleTostring(cpuRate.guest_nice*100, 2);
        (*root)[cpuRate.id] = cpu;
    }
    delete cpu_before;
    cpu_before = new Cpu(cpu);

    Json::FastWriter writer;
    string json_str = writer.write(*root);
    delete root;

    post(json_str);
}

void PostData::post_meminfo() {
    Json::Value* mem = NULL;
    MemInfo meminfo = perf_data->memInfo_buffer.fetch();

    mem = new Json::Value();
    (*mem)["type"] = "mem";
    (*mem)["time"] = tostring(meminfo.time_stamp);
    (*mem)["memTotal"] = tostring(meminfo.memTotal);
    (*mem)["memFree"] = tostring(meminfo.memFree);
    (*mem)["buffers"] = tostring(meminfo.buffers);
    (*mem)["cached"] = tostring(meminfo.cached);
    (*mem)["swapTotal"] = tostring(meminfo.swapTotal);
    (*mem)["swapFree"] = tostring(meminfo.swapFree);

    Json::FastWriter writer;
    string json_str = writer.write(*mem);
    delete mem;

    post(json_str);
    
}

void PostData::post_vmstat() {
    Json::Value* disk = NULL;
    Vmstat vmstat = perf_data->vmstat_buffer.fetch();

    if(vmstat_before == NULL) {
        vmstat_before = new Vmstat(vmstat);
        return;
    }

    disk = new Json::Value();
    (*disk)["type"] = "disk";
    Vmstat vmstatDelta = vmstat - *vmstat_before;
    (*disk)["time"] = tostring(vmstat.time_stamp);
    (*disk)["read"] = tostring(vmstatDelta.pgpgin * perf_data->page_size);
    (*disk)["write"] = tostring(vmstatDelta.pgpgout * perf_data->page_size);

    delete vmstat_before;
    vmstat_before = new Vmstat(vmstat);

    Json::FastWriter writer;
    string json_str = writer.write(*disk);
    delete disk;

    post(json_str);

}

void PostData::post_netstat() {
    Json::Value* net = NULL;
    Netstat netstat = perf_data->netstat_buffer.fetch();

    if(netstat_before == NULL) {
        netstat_before = new Netstat(netstat);
        return;
    }

    net = new Json::Value();
    (*net)["type"] = "net";
    Netstat netstatDelta = netstat - *netstat_before;
    (*net)["time"] = tostring(netstat.time_stamp);
    (*net)["receive"] = tostring(netstatDelta.InOctets);
    (*net)["send"] = tostring(netstatDelta.OutOctets);

    delete netstat_before;
    netstat_before = new Netstat(netstat);

    Json::FastWriter writer;
    string json_str = writer.write(*net);
    delete net;

    post(json_str);
}

void PostData::post_traffic() {
    std::list<std::pair<IP::address_type, Traffic> > traffic_list = perf_data->trafficData.fetch();
    Json::Value* traffic_json = NULL;
    traffic_json = new Json::Value();
    (*traffic_json)["type"] = "traffic";

    for(auto it = traffic_list.begin(); it != traffic_list.end(); ++it) {
        Traffic& traffic = it->second;

        Json::Value ip;
        ip["speed_in"] = tostring(traffic.getSpeedIn());
        ip["speed_out"] = tostring(traffic.getSpeedOut());
        ip["total_in"] = tostring(traffic.getTotalIn());
        ip["total_out"] = tostring(traffic.getTotalOut());
        
        (*traffic_json)[traffic.getAddress()] = ip;
    }

    Json::FastWriter writer;
    string json_str = writer.write(*traffic_json);
    delete traffic_json;

    post(json_str);
}


void PostData::post_cpu_task() {
    while(true) {
        post_cpu();
    }
}
void PostData::post_meminfo_task() {
    while(true) {
        post_meminfo();
    }
}
void PostData::post_vmstat_task() {
    while(true) {
        post_vmstat();
    }
}
void PostData::post_netstat_task() {
    while(true) {
        post_netstat();
    }
}
void PostData::post_traffic_task() {
    while(true) {
        post_traffic();
    }
}

void PostData::start() {
    thread cpu_thread(bind(&PostData::post_cpu_task, this));
    thread mem_thread(bind(&PostData::post_meminfo_task, this));
    thread disk_thread(bind(&PostData::post_vmstat_task, this));
    thread net_thread(bind(&PostData::post_netstat_task, this));
    thread traffic_thread(bind(&PostData::post_traffic_task, this));

    cpu_thread.join();
    mem_thread.join();
    disk_thread.join();
    net_thread.join();
    traffic_thread.join();
}
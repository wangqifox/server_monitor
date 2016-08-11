#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include <thread>

#include "post_data.h"
#include "proc_main.h"
#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"
#include "utils.h"
#include "json/json.h"
using namespace std;

void ServerData::post(string data) {
    for (auto it : *m_connections) {
        server::connection_ptr con = m_server->get_con_from_hdl(it);
        con->send(data);
    }
}

void ServerData::add_cpu(Cpu& cpu) {
    cpu_repository.add(cpu);
}

void ServerData::add_meminfo(MemInfo& meminfo) {
    memInfo_repository.add(meminfo);
}

void ServerData::add_vmstat(Vmstat& vmstat) {
    vmstat_repository.add(vmstat);
}

void ServerData::add_netstat(Netstat& netstat) {
    netstat_repository.add(netstat);
}


void ServerData::post_cpu() {
    Json::Value* root = NULL;
    Cpu cpu = cpu_repository.fetch();

    if(cpu_before == NULL) {
        cpu_before = new Cpu(cpu);
        return;
    } 

    vector<CpuRate> cpuRateVec = cpuRate(*cpu_before, cpu);

    root = new Json::Value();
    (*root)["type"] = "cpu";
    (*root)["time"] = tostring(cpu.time);
    for(CpuRate cpuRate : cpuRateVec){
        Json::Value cpu;
        cpu["id"] = cpuRate.id;
        cpu["total"] = doubleTostring(cpuRate.total*100, 2);
        cpu["user"] = doubleTostring(cpuRate.user*100, 2);
        cpu["nice"] = doubleTostring(cpuRate.nice*100, 2);
        cpu["system"] = doubleTostring(cpuRate.system*100, 2);
        cpu["idle"] = doubleTostring(cpuRate.idle*100, 2);
        cpu["iowait"] = doubleTostring(cpuRate.iowait*100, 2);
        cpu["irq"] = doubleTostring(cpuRate.irq*100, 2);
        cpu["softirq"] = doubleTostring(cpuRate.softirq*100, 2);
        cpu["steal"] = doubleTostring(cpuRate.steal*100, 2);
        cpu["guest"] = doubleTostring(cpuRate.guest*100, 2);
        cpu["guest_nice"] = doubleTostring(cpuRate.guest_nice*100, 2);
        (*root)[cpuRate.id] = cpu;
    }
    delete cpu_before;
    cpu_before = new Cpu(cpu);

    Json::FastWriter writer;
    string json_str = writer.write(*root);
    delete root;

    post(json_str);
}

void ServerData::post_meminfo() {
    Json::Value* mem = NULL;
    MemInfo meminfo = memInfo_repository.fetch();

    mem = new Json::Value();
    (*mem)["type"] = "mem";
    (*mem)["time"] = tostring(meminfo.time);
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

void ServerData::post_vmstat() {
    Json::Value* disk = NULL;
    Vmstat vmstat = vmstat_repository.fetch();

    if(vmstat_before == NULL) {
        vmstat_before = new Vmstat(vmstat);
        return;
    }

    disk = new Json::Value();
    (*disk)["type"] = "disk";
    Vmstat vmstatDelta = vmstat - *vmstat_before;
    (*disk)["time"] = tostring(vmstat.time);
    (*disk)["read"] = tostring(vmstatDelta.pgpgin * page_size);
    (*disk)["write"] = tostring(vmstatDelta.pgpgout * page_size);

    delete vmstat_before;
    vmstat_before = new Vmstat(vmstat);

    Json::FastWriter writer;
    string json_str = writer.write(*disk);
    delete disk;

    post(json_str);

}

void ServerData::post_netstat() {
    Json::Value* net = NULL;
    Netstat netstat = netstat_repository.fetch();

    if(netstat_before == NULL) {
        netstat_before = new Netstat(netstat);
        return;
    }

    net = new Json::Value();
    (*net)["type"] = "net";
    Netstat netstatDelta = netstat - *netstat_before;
    (*net)["time"] = tostring(netstat.time);
    (*net)["receive"] = tostring(netstatDelta.InOctets);
    (*net)["send"] = tostring(netstatDelta.OutOctets);

    delete netstat_before;
    netstat_before = new Netstat(netstat);

    Json::FastWriter writer;
    string json_str = writer.write(*net);
    delete net;

    post(json_str);
}

void ServerData::post_traffic() {
    // std::cout << "======traverse========" << std::endl;
    // trafficData.traverse();
    std::list<std::pair<IP::address_type, Traffic> >& traffic_list = trafficData.getList();
    Json::Value* traffic_json = NULL;
    traffic_json = new Json::Value();
    (*traffic_json)["type"] = "traffic";

    for(auto it = traffic_list.begin(); it != traffic_list.end(); ++it) {
        // std::cout << it->first << " : " << it->second << std::endl;
        Traffic& traffic = it->second;


        Json::Value ip;
        ip["speed_in"] = traffic.getSpeedIn();
        ip["speed_out"] = traffic.getSpeedOut();
        ip["total_in"] = traffic.getTotalIn();
        ip["total_out"] = traffic.getTotalOut();
        

        (*traffic_json)[traffic.getAddress()] = ip;
        
        // cout << traffic.getAddress() << endl;
    }

    Json::FastWriter writer;
    string json_str = writer.write(*traffic_json);
    cout << json_str << endl;
    delete traffic_json;
    post(json_str);

    sleep(1);
}


void cpu_info_task(ServerData* serverData) {
    while(true){
        serverData->post_cpu();
    }
}

void mem_info_task(ServerData* serverData) {
    while(true){
        serverData->post_meminfo();
    }
    
}

void disk_info_task(ServerData* serverData) {
    while(true){
        serverData->post_vmstat();
    }
    
}

void net_info_task(ServerData* serverData) {
    while(true){
        serverData->post_netstat();
    }
    
}

void traffic_info_task(ServerData* serverData) {
    while(true) {
        serverData->post_traffic();
    }
}

void post_data(ServerData* serverData, int delay) {
    // thread proc_thread(proc_main, serverData, delay);

    thread cpu_thread(cpu_info_task, serverData);
    thread mem_thread(mem_info_task, serverData);
    thread disk_thread(disk_info_task, serverData);
    thread net_thread(net_info_task, serverData);
    thread traffic_thread(traffic_info_task, serverData);

    // proc_thread.join();
    cpu_thread.join();
    mem_thread.join();
    disk_thread.join();
    net_thread.join();
    traffic_thread.join();

}


#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <curl/curl.h>

#include "post_data.h"
#include "cpu.h"
#include "meminfo.h"
#include "vmstat.h"
#include "netstat.h"
#include "utils.h"
using namespace std;

void post(const char * url, string project_key, string data) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        string str("project_key="+project_key+"&data="+data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str.c_str());
        res = curl_easy_perform(curl);
        // cout << res << endl;
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

Json::Value* ServerData::cpu_info() {
    Json::Value* root = NULL;
    if(!cpu_queue.empty()){
        Cpu cpu = cpu_queue.front();
        cpu_queue.pop();

        if(cpu_before == NULL) {
            cpu_before = new Cpu();
            *cpu_before = cpu;
            return NULL;
        }

        vector<CpuRate> cpuRateVec = cpuRate(*cpu_before, cpu);
        
        root = new Json::Value();
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
        cpu_before = new Cpu();
        *cpu_before = cpu;

        Json::FastWriter writer;
        string json_str = writer.write(*root);
        cout << json_str << endl;

        string url = post_url + "/cpu";
        post(url.c_str(), project_key, json_str);
    }
    return root;
}

Json::Value* ServerData::mem_info() {
    Json::Value* mem = NULL;
    if(!memInfo_queue.empty()){
        MemInfo meminfo = memInfo_queue.front();
        // cout << meminfo << endl;
        memInfo_queue.pop();
        mem = new Json::Value();
        (*mem)["time"] = tostring(meminfo.time);
        (*mem)["memTotal"] = tostring(meminfo.memTotal);
        (*mem)["memFree"] = tostring(meminfo.memFree);
        (*mem)["buffers"] = tostring(meminfo.buffers);
        (*mem)["cached"] = tostring(meminfo.cached);
        (*mem)["swapTotal"] = tostring(meminfo.swapTotal);
        (*mem)["swapFree"] = tostring(meminfo.swapFree);

        Json::FastWriter writer;
        string json_str = writer.write(*mem);
        cout << json_str << endl;

        string url = post_url + "/mem";
        post(url.c_str(), project_key, json_str);
    }
    return mem;
}

Json::Value* ServerData::disk_info() {
    Json::Value* disk = NULL;
    if(!vmstat_queue.empty()){
        Vmstat vmstat = vmstat_queue.front();
        // cout << vmstat << endl;
        vmstat_queue.pop();

        if(vmstat_before == NULL) {
            vmstat_before = new Vmstat();
            *vmstat_before = vmstat;
            return NULL;
        }

        disk = new Json::Value();
        Vmstat vmstatDelta = vmstat - *vmstat_before;
        // cout << vmstatDelta << endl;
        (*disk)["time"] = tostring(vmstat.time);
        (*disk)["read"] = tostring(vmstatDelta.pgpgin * page_size);
        (*disk)["write"] = tostring(vmstatDelta.pgpgout * page_size);

        vmstat_before = new Vmstat();
        *vmstat_before = vmstat;

        Json::FastWriter writer;
        string json_str = writer.write(*disk);
        cout << json_str << endl;

        string url = post_url + "/disk";
        post(url.c_str(), project_key, json_str);
    }
    return disk;
}

Json::Value* ServerData::net_info() {
    Json::Value* net = NULL;
    if(!netstat_queue.empty()){
        Netstat netstat = netstat_queue.front();
        // cout << netstat << endl;
        netstat_queue.pop();

        if(netstat_before == NULL) {
            netstat_before = new Netstat();
            *netstat_before = netstat;
        }

        net = new Json::Value();
        Netstat netstatDelta = netstat - *netstat_before;
        (*net)["time"] = tostring(netstat.time);
        (*net)["receive"] = tostring(netstatDelta.InOctets);
        (*net)["send"] = tostring(netstatDelta.OutOctets);

        netstat_before = new Netstat();
        *netstat_before = netstat;

        Json::FastWriter writer;
        string json_str = writer.write(*net);
        cout << json_str << endl;

        string url = post_url + "/net";
        post(url.c_str(), project_key, json_str);
    }
    return net;
}

void post_data() {
    ServerData serverData;
    while(true) {
        Json::Value* cpu_info = serverData.cpu_info();
        Json::Value* mem_info = serverData.mem_info();
        Json::Value* disk_info = serverData.disk_info();
        Json::Value* net_info = serverData.net_info();
        sleep(1);
    }
}





#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sstream>
#include <curl/curl.h>
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

size_t ServerData::write_data(void *ptr, size_t size, size_t nmemb, url_data *data) {
    size_t n = size * nmemb;
    free(data->data);
    data->data = (char*) malloc(sizeof(char)*(n+1));
    if(!data->data) {
        cout << "Failed to allocate memory" << endl;
        return 0;
    }
    memcpy(data->data, ptr, n);
    data->data[n] = '\0';

    string str(data->data);
    // cout << str << endl;
    return n;
}

void ServerData::post(string data) {
    // CURL *curl;
    // CURLcode res;

    // curl_global_init(CURL_GLOBAL_ALL);

    // curl = curl_easy_init();
    // if(curl) {
    //     curl_easy_setopt(curl, CURLOPT_URL, url);
    //     string str("project_key="+project_key+"&"+"data="+data);
    //     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str.c_str());
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    //     url_data urldata;
    //     urldata.size = 0;
    //     urldata.data = NULL;
    //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &urldata);
    //     res = curl_easy_perform(curl);
    //     // cout << res << endl;
    //     curl_easy_cleanup(curl);
    // }
    // curl_global_cleanup();

    for (auto it : *m_connections) {
        // m_server.send(*it, ss.str());
        // websocketpp::connection_hdl hdl = it;
        // cout << &hdl << endl;
        server::connection_ptr con = m_server->get_con_from_hdl(it);
        con->send(data);
    }

}

void ServerData::add_cpu(Cpu& cpu) {
    unique_lock<mutex> lock(cpu_queue_mtx);
    cpu_queue.push(cpu);
    lock.unlock();
    cpu_queue_not_empty.notify_all();
    
}

void ServerData::add_meminfo(MemInfo& meminfo) {
    unique_lock<mutex> lock(memInfo_queue_mtx);
    memInfo_queue.push(meminfo);
    lock.unlock();
    memInfo_queue_not_empty.notify_all();
    
    
}

void ServerData::add_vmstat(Vmstat& vmstat) {
    unique_lock<mutex> lock(vmstat_queue_mtx);
    vmstat_queue.push(vmstat);
    lock.unlock();
    vmstat_queue_not_empty.notify_all();
    
    
}

void ServerData::add_netstat(Netstat& netstat) {
    unique_lock<mutex> lock(netstat_queue_mtx);
    netstat_queue.push(netstat);
    lock.unlock();
    netstat_queue_not_empty.notify_all();
    
    
}


void ServerData::post_cpu() {
    Json::Value* root = NULL;
    unique_lock<mutex> lock(cpu_queue_mtx);
    if(cpu_queue.empty()) {
        cpu_queue_not_empty.wait(lock);
        lock.unlock();
    } else {
        lock.unlock();
    }
    
    if(!cpu_queue.empty()){
        unique_lock<mutex> lock(cpu_queue_mtx);
        Cpu cpu = cpu_queue.front();
        // cout << "cpu " << cpu << endl;
        cpu_queue.pop();
        lock.unlock();

        if(cpu_before == NULL) {
            cpu_before = new Cpu();
            *cpu_before = cpu;
        } else {
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

            // string url = post_url + "/cpu";
            post(json_str);

            // con->send(json_str);
        }
    }
    
}

void ServerData::post_meminfo() {
    Json::Value* mem = NULL;
    unique_lock<mutex> lock(memInfo_queue_mtx);
    if(memInfo_queue.empty()) {
        memInfo_queue_not_empty.wait(lock);
        lock.unlock();
    } else {
        lock.unlock();
    }

    if(!memInfo_queue.empty()){
        unique_lock<mutex> lock(memInfo_queue_mtx);
        MemInfo meminfo = memInfo_queue.front();
        // cout << meminfo << endl;
        memInfo_queue.pop();
        lock.unlock();

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
        // cout << json_str << endl;

        // string url = post_url + "/mem";
        post(json_str);
    }
    
}

void ServerData::post_vmstat() {
    Json::Value* disk = NULL;
    unique_lock<mutex> lock(vmstat_queue_mtx);
    if(vmstat_queue.empty()) {
        vmstat_queue_not_empty.wait(lock);
        lock.unlock();
    } else {
        lock.unlock();
    }

    if(!vmstat_queue.empty()){
        unique_lock<mutex> lock(vmstat_queue_mtx);
        Vmstat vmstat = vmstat_queue.front();
        // cout << vmstat << endl;
        vmstat_queue.pop();
        lock.unlock();

        if(vmstat_before == NULL) {
            vmstat_before = new Vmstat();
            *vmstat_before = vmstat;
            return;
        }

        disk = new Json::Value();
        Vmstat vmstatDelta = vmstat - *vmstat_before;
        (*disk)["time"] = tostring(vmstat.time);
        (*disk)["read"] = tostring(vmstatDelta.pgpgin * page_size);
        (*disk)["write"] = tostring(vmstatDelta.pgpgout * page_size);

        vmstat_before = new Vmstat();
        *vmstat_before = vmstat;

        Json::FastWriter writer;
        string json_str = writer.write(*disk);
        // cout << json_str << endl;

        // string url = post_url + "/disk";
        post(json_str);
    }
}

void ServerData::post_netstat() {
    Json::Value* net = NULL;
    unique_lock<mutex> lock(netstat_queue_mtx);
    if(netstat_queue.empty()) {
        netstat_queue_not_empty.wait(lock);
        lock.unlock();
    } else {
        lock.unlock();
    }

    if(!netstat_queue.empty()){
        unique_lock<mutex> lock(netstat_queue_mtx);
        Netstat netstat = netstat_queue.front();
        // cout << netstat << endl;
        netstat_queue.pop();
        lock.unlock();

        if(netstat_before == NULL) {
            netstat_before = new Netstat();
            *netstat_before = netstat;
            return;
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
        // cout << json_str << endl;

        // string url = post_url + "/net";
        post(json_str);
    }
}


void cpu_info_task(ServerData* serverData) {
    while(true){
        cout << "cpu_info_task" << endl;
        serverData->post_cpu();
        // usleep(800*1000);
    }
}

void mem_info_task(ServerData* serverData) {
    while(true){
        // cout << "mem_info_task" << endl;
        serverData->post_meminfo();
        // usleep(800*1000);
    }
    
}

void disk_info_task(ServerData* serverData) {
    while(true){
        // cout << "disk_info_task" << endl;
        serverData->post_vmstat();
        // usleep(800*1000);
    }
    
}

void net_info_task(ServerData* serverData) {
    while(true){
        // cout << "net_info_task" << endl;
        serverData->post_netstat();
        // usleep(800*1000);
    }
    
}

void post_data(ServerData* serverData) {

    thread proc_thread(proc_main, serverData);
    thread cpu_thread(cpu_info_task, serverData);
    thread mem_thread(mem_info_task, serverData);
    thread disk_thread(disk_info_task, serverData);
    thread net_thread(net_info_task, serverData);

    proc_thread.join();
    cpu_thread.join();
    mem_thread.join();
    disk_thread.join();
    net_thread.join();

}


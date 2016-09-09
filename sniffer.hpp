#ifndef SNIFFER_HPP
#define SNIFFER_HPP

#include <iostream>
#include <tins/tins.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <list>
#include <mutex>
#include "perf_data.hpp"

using namespace Tins;

class TrafficSniffer {
private:
    PerfData* perf_data;
    Sniffer* _sniffer;
    IP::address_type ip_addr;
    time_t seconds;

public:
    TrafficSniffer(PerfData* perf_data):perf_data(perf_data){}

    bool callback(const Packet &packet) {
        time_t now = packet.timestamp().seconds();
        if (now != seconds) {
            perf_data->trafficData.read_to_fetch.notify_all();
            seconds = now;
        } 
        const PDU* pdu = packet.pdu();
        const IP &ip = pdu->rfind_pdu<IP>();


        if(ip_addr != ip.src_addr()) {
            perf_data->trafficData.addIn(ip.src_addr(), ip.tot_len());
        }
        
        if(ip_addr != ip.dst_addr()) {
            perf_data->trafficData.addOut(ip.dst_addr(), ip.tot_len());
        }

        return true;
    }

    void start_sniffer() {
        try {
            NetworkInterface iface = NetworkInterface("eth0");
            NetworkInterface::Info info = iface.info();
            ip_addr = info.ip_addr;
            
            _sniffer = new Sniffer("eth0");
        } catch(const std::exception& e) {
            // std::cout << "not root, traffic monitor cannot setup " << e.what() << std::endl;
            return;
        }
        _sniffer->sniff_loop(std::bind(&TrafficSniffer::callback, this, std::placeholders::_1));
    }
};

#endif
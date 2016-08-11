#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <exception>
#include <stdexcept>

#include "websocket_server.h"

using namespace std;

void WebsocketServer::on_message(connection_hdl hdl, message_ptr msg) {

}


void WebsocketServer::process_messages() {
    post_data(serverData, delay);
}

void WebsocketServer::on_open(connection_hdl hdl) {
    cout << "on_open" << endl;
    cout << &hdl << endl;
    m_connections.insert(hdl);

    // server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    // con->send("opend");

}


void WebsocketServer::on_close(connection_hdl hdl) {
    cout << "on_close" << endl;
    m_connections.erase(hdl);
}


void WebsocketServer::traverse() {
    while(true) {
        std::cout << "==============" << std::endl;
        serverData->trafficData.traverse();
        sleep(1);
    }
}

bool WebsocketServer::callback(const Packet &packet) {
    // std::cout << packet.timestamp().seconds() << std::endl;
    time_t now = packet.timestamp().seconds();
    if (now != seconds) {
        serverData->trafficData.clearSpeed();
        seconds = now;
    } 
    const PDU* pdu = packet.pdu();
    const IP &ip = pdu->rfind_pdu<IP>();
    // std::cout << ip.src_addr() << " -> "
    //        << ip.dst_addr() << " "
    //        << ip.tot_len() << std::endl;

    if(ip_addr != ip.src_addr()) {
        serverData->trafficData.addIn(ip.src_addr(), ip.tot_len());
    }
    
    if(ip_addr != ip.dst_addr()) {
        serverData->trafficData.addOut(ip.dst_addr(), ip.tot_len());
    }

    return true;
}

void WebsocketServer::start_sniffer() {
    NetworkInterface iface = NetworkInterface("eth0");
    std::cout << iface.name() << std::endl;
    NetworkInterface::Info info = iface.info();
    std::cout << info.ip_addr << std::endl;
    ip_addr = info.ip_addr;
    
    _sniffer.sniff_loop(std::bind(&WebsocketServer::callback, this, std::placeholders::_1));
    // while(true) {
    //     cout << "++++++++++++++sniffer++++++++++++" << endl;
    //     sleep(1);
    // }
}

void WebsocketServer::start_proc() {
    while(true) {
        cout << "start_proc" << endl;
        Cpu cpu = readCpuStat();
        MemInfo meminfo = readMemInfo();
        Vmstat vmstat = readVmstat();
        Netstat netstat = readNetstat();
        
        serverData->add_cpu(cpu);
        serverData->add_meminfo(meminfo);
        serverData->add_vmstat(vmstat);
        serverData->add_netstat(netstat);

        sleep(delay);
    }

}

void WebsocketServer::run() {
    try {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
        
    } catch(websocketpp::exception const & e) {
        cout << "websocketpp exception " << e.what() << endl;
        exit(0);
    } catch(const std::exception& e) {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }
    // catch(...) {
    //     cout << "other exception" << endl;
    //     exit(0);
    // }
}


void start_server(int port, int delay) {

    try {
        WebsocketServer websocket_server(port, delay);
        cout << "listening " << port << endl;
        thread process_thread(bind(&WebsocketServer::process_messages, &websocket_server));
        thread sniffer_thread(bind(&WebsocketServer::start_sniffer, &websocket_server));
        thread proc_thread(bind(&WebsocketServer::start_proc, &websocket_server));

        websocket_server.run();
        process_thread.join();
        sniffer_thread.join();
        proc_thread.join();
        
    } catch(const std::exception& e) {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }

    // catch(...) {
    //     cout << "other exception" << endl;
    //     exit(0);
    // }

}


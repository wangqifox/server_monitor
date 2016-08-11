#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <set>

#include "proc_main.h"
#include "post_data.h"
#include "sniffer.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using namespace std;

typedef server::message_ptr message_ptr;



class WebsocketServer{
private:
    server m_server;
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;
    con_list m_connections;

    void on_message(connection_hdl hdl, message_ptr msg);
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);

    ServerData *serverData;

    Sniffer _sniffer = Sniffer("eth0");
    IP::address_type ip_addr;
    time_t seconds;

    int port;
    int delay;
public:
    void run();
    void process_messages();
    bool callback(const Packet &packet);
    void traverse();
    void start_sniffer();
    void start_proc();
    
    WebsocketServer(int port, int delay) {
        this->port = port;
        this->delay = delay;
        serverData = new ServerData(&m_server, &m_connections);

        m_server.clear_access_channels(websocketpp::log::alevel::all);
        m_server.set_access_channels(websocketpp::log::alevel::connect);
        m_server.set_access_channels(websocketpp::log::alevel::disconnect);
        // m_server.set_access_channels(websocketpp::log::alevel::app);
        
        m_server.init_asio();

        m_server.set_open_handler(bind(&WebsocketServer::on_open,this,::_1));
        m_server.set_close_handler(bind(&WebsocketServer::on_close,this,::_1));
        m_server.set_message_handler(bind(&WebsocketServer::on_message,this,::_1,::_2));
    }

};

#endif

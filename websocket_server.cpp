#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <sstream>
#include <thread>

#include "websocket_server.h"

using namespace std;

void WebsocketServer::on_message(connection_hdl hdl, message_ptr msg) {

}


void WebsocketServer::process_messages() {
    post_data(serverData);
}

void WebsocketServer::on_open(connection_hdl hdl) {
    cout << "on_open" << endl;
    cout << &hdl << endl;
    m_connections.insert(hdl);

    server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    con->send("opend");

}


void WebsocketServer::on_close(connection_hdl hdl) {
    cout << "on_close" << endl;
    m_connections.erase(hdl);
}

void WebsocketServer::run() {
    try {
        m_server.listen(9002);
        m_server.start_accept();
        m_server.run();
    } catch(websocketpp::exception const & e) {
        cout << e.what() << endl;
    } catch(...) {
        cout << "other exception" << endl;
    }
}


void start_server() {

    try {
        WebsocketServer websocket_server;

        thread t(bind(&WebsocketServer::process_messages,&websocket_server));
        websocket_server.run();
        t.join();
    }catch(...) {
        cout << "other exception" << endl;
    }
    
}
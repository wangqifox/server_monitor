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

void WebsocketServer::on_open(connection_hdl hdl) {
    m_connections.insert(hdl);
}


void WebsocketServer::on_close(connection_hdl hdl) {
    m_connections.erase(hdl);
}

void WebsocketServer::run() {
    try {
        m_server.listen(boost::asio::ip::tcp::v4(), port);
        m_server.start_accept();
        m_server.run();
        
    } catch(websocketpp::exception const & e) {
        cout << "websocketpp exception " << e.what() << endl;
        exit(0);
    } catch(const std::exception& e) {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }
}


void WebsocketServer::send_data(string data) {
    for (auto it : m_connections) {
        server::connection_ptr con = m_server.get_con_from_hdl(it);
        con->send(data);
    }
}
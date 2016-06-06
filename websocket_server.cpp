#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <sstream>
#include <thread>

#include "websocket_server.h"

using namespace std;

void WebsocketServer::on_message(connection_hdl hdl, message_ptr msg) {
    // cout << "on_message called with hdl: " << hdl.lock().get()
    //     << " and message: " << msg->get_payload()
    //     << endl;

    // if (msg->get_payload() == "stop-listening") {
    //     s->stop_listening();
    //     return;
    // }

    // try {
    //     s->send(hdl, msg->get_payload(), msg->get_opcode());
    // } catch(const websocketpp::lib::error_code& e) {
    //     cout << "Echo failed because: " << e
    //         << "(" << e.message() << ")" << endl;
    // }
}

// void send_task(server::connection_ptr con) {
//     for (int i = 0 ; i < 10 ; i++) {
//         stringstream ss;
//         ss << "hello " << i;
//         con->send(ss.str());
//     }
// }

void WebsocketServer::process_messages() {
    // for (int i = 0 ; i < 10 ; i++) {
    //     stringstream ss;
    //     ss << "hello " << i;
    //     cout << ss.str() << endl;
    //     // con->send(ss.str());
    //     for (auto it : m_connections) {
    //         // m_server.send(*it, ss.str());
    //         // websocketpp::connection_hdl hdl = it;
    //         // cout << &hdl << endl;
    //         server::connection_ptr con = m_server.get_con_from_hdl(it);
    //         con->send(ss.str());

    //         // m_server.send(it, ss.str());
    //     }
    //     sleep(1);
    // }
    // thread proc_thread(proc_main, serverData);
    post_data(serverData);
    // proc_thread.join();
}

void WebsocketServer::on_open(connection_hdl hdl) {
    cout << "on_open" << endl;
    // server::connection_ptr con = s->get_con_from_hdl(hdl);
    cout << &hdl << endl;
    m_connections.insert(hdl);

    server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    con->send("opend");
    
    // ifstream ifs;
    // ifs.open(".config");

    // Json::Reader reader;
    // Json::Value root;
    // if(!reader.parse(ifs, root, false)) {
    //     cout << "can not find .config file" << endl;
    //     exit(1);
    // }
    // string url = root["url"].asString();
    // string key = root["key"].asString();

    // cout << url << endl;
    // cout << key << endl;

    // ServerData *serverData = new ServerData(url,  key);

    // thread proc_thread(proc_main, serverData);
    // // thread post_thread(post_data, serverData);
    // // post_data(serverData);

    // // thread server_thread(start_server);
    // // proc_thread.join();

    // thread send_thread(post_data, serverData, con);
    // send_thread.join();

    // for (int i = 0 ; i < 10 ; i++) {
    //     stringstream ss;
    //     ss << "hello " << i;
    //     con->send(ss.str());
    // }

    // thread proc_thread(proc_main, serverData);
    // thread post_thread(send_message, serverData, con);
    // post_thread.join();
    // proc_thread.join();
    

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
#include <iostream>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include "websocket_server.h"
#include "httpd.hpp"
#include "perf_data.hpp"
#include "sniffer.hpp"
#include "proc_main.hpp"
#include "progress_main.hpp"
#include "post_data.h"
using namespace std;

const static string VERSION = "version 1.3";

void start_server(int port, int delay) {
    WebsocketServer* websocket_server;
    try {
        websocket_server = new WebsocketServer(port);
        cout << "websocket listening on port " << port << endl;
    }catch(const std::exception& e) {
        std::cout << "WebsocketServer \"" << e.what() << "\"\n";
    }

    PerfData* perf_data = new PerfData();

    TrafficSniffer* sniffer = new TrafficSniffer(perf_data);
    PostData* post_data = new PostData(websocket_server, perf_data);
    HttpServer* http_server = new HttpServer(port+1);

    thread sniffer_thread;
    thread post_thread;
    thread proc_thread;
    thread progress_thread;
    thread http_thread;

    try {
        sniffer_thread = thread(bind(&TrafficSniffer::start_sniffer, sniffer));

        post_thread = thread(bind(&PostData::start, post_data));

        proc_thread = thread(start_proc, perf_data, delay);

        progress_thread = thread(start_progress_monitor, perf_data, delay);

        http_thread = thread(bind(&HttpServer::start, http_server));
    } catch(const std::exception& e) {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }

    websocket_server->run();
    post_thread.join();
    sniffer_thread.join();
    proc_thread.join();
    progress_thread.join();
    http_thread.join();
}

int main(int argc, char **argv) {
    int port = 9002;
    int delay = 1;
    char ch;
    while((ch = getopt(argc, argv, "p:t:v")) != EOF) {
        switch(ch) {
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                delay = atoi(optarg);
                break;
            case 'v':
                cout << VERSION << endl;
                exit(0);
        }
    }

    thread server_thread(start_server, port, delay);
    server_thread.join();
    return 0;
}


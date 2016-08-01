#include <iostream>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include "proc_main.h"
#include "post_data.h"
using namespace std;

extern void start_server(int port, int delay);
extern void http_server();

int main(int argc, char **argv) {
    int port = 9002;
    int delay = 1;
    char ch;
    while((ch = getopt(argc, argv, "pt:")) != EOF) {
        switch(ch) {
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                delay = atoi(optarg);
                break;
        }
    }

    
    thread http_thread(http_server);
    // http_thread.join();
    thread server_thread(start_server, port, delay);
    http_thread.join();
    server_thread.join();
    return 0;
}


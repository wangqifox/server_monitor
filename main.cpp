#include <iostream>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

#include "proc_main.h"
#include "post_data.h"
using namespace std;

extern void start_server(int port, int delay);

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

    cout << "listening " << port << endl;
    thread server_thread(start_server, port, delay);
    server_thread.join();
    return 0;
}


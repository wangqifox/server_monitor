#include <iostream>
#include <thread>
#include <fstream>

#include "proc_main.h"
#include "post_data.h"
using namespace std;

extern void start_server();

int main() {
    

    // thread proc_thread(proc_main, serverData);
    // thread post_thread(post_data, serverData);
    // post_data(serverData);

    thread server_thread(start_server);
    // proc_thread.join();
    server_thread.join();
    return 0;
}


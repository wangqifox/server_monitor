#include <iostream>
#include <thread>
#include "proc_main.h"
#include "post_data.h"
using namespace std;

int main() {
    thread proc_thread(proc_main);
    thread post_thread(post_data);
    post_thread.join();
    return 0;
}

#include <iostream>
#include <thread>
#include <fstream>

#include "proc_main.h"
#include "post_data.h"
using namespace std;

int main() {
    ifstream ifs;
    ifs.open(".config");

    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(ifs, root, false)) {
        cout << "can not find .config file" << endl;
        exit(1);
    }
    string url = root["url"].asString();
    string key = root["key"].asString();

    cout << url << endl;
    cout << key << endl;

    ServerData *serverData = new ServerData(url,  key);

    thread proc_thread(proc_main, serverData);
    // thread post_thread(post_data, serverData);
    post_data(serverData);
    proc_thread.join();
    return 0;
}

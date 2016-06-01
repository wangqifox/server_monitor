#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.h"

using namespace std;

const string NETSTATFILE = "/proc/net/netstat";

Netstat readNetstat() {
    ifstream in(NETSTATFILE);
    string line;
    Netstat netstat;
    netstat.time = time(NULL);
    if(in){
        getline(in, line);
        if(line.find("TcpExt") != 0) return netstat;
        getline(in, line);
        if(line.find("TcpExt") != 0) return netstat;
        getline(in, line);
        if(line.find("IpExt") != 0) return netstat;
        getline(in, line);
        if(line.find("IpExt") != 0) return netstat;
        vector<string> words = getWords(line);
        netstat.InNoRoutes = stoull(words[1]);
        netstat.InTruncatedPkts = stoull(words[2]);
        netstat.InMcastPkts = stoull(words[3]);
        netstat.OutMcastPkts = stoull(words[4]);
        netstat.InBcastPkts = stoull(words[5]);
        netstat.OutBcastPkts = stoull(words[6]);
        netstat.InOctets = stoull(words[7]);
        netstat.OutOctets = stoull(words[8]);
        netstat.InMcastOctets = stoull(words[9]);
        netstat.OutMcastOctets = stoull(words[10]);
        netstat.InBcastOctets = stoull(words[11]);
        netstat.OutBcastOctets = stoull(words[12]);
    }
    in.close();
    return netstat;
}

int main_netstat() {
    Netstat netstat1 = readNetstat();
    sleep(1);
    Netstat netstat2 = readNetstat();

    cout << netstat1 << endl;
    cout << netstat2 << endl;

    Netstat netstat = netstat2 - netstat1;
    cout << netstat.InOctets * 8 / 1000.0 << endl;
    cout << netstat.OutOctets * 8 / 1000.0 << endl;

    return 0;
}
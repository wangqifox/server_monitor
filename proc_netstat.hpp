#ifndef NETSTAT_HPP
#define NETSTAT_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "utils.hpp"

using namespace std;

const string NETSTATFILE = "/proc/net/netstat";

class Netstat {
public:
    time_t time_stamp;
    unsigned long long InNoRoutes;
    unsigned long long InTruncatedPkts;
    unsigned long long InMcastPkts;
    unsigned long long OutMcastPkts;
    unsigned long long InBcastPkts;
    unsigned long long OutBcastPkts;
    unsigned long long InOctets;
    unsigned long long OutOctets;
    unsigned long long InMcastOctets;
    unsigned long long OutMcastOctets;
    unsigned long long InBcastOctets;
    unsigned long long OutBcastOctets;

    Netstat():time_stamp(0),InNoRoutes(0),InTruncatedPkts(0),InMcastPkts(0),OutMcastPkts(0),InBcastPkts(0),OutBcastPkts(0),
                InOctets(0),OutOctets(0),InMcastOctets(0),OutMcastOctets(0),InBcastOctets(0),OutBcastOctets(0){}

    friend ostream& operator<< (ostream &os, Netstat &netstat) {
        os << netstat.time_stamp << " " 
            << netstat.InNoRoutes << " " 
            << netstat.InTruncatedPkts << " " 
            << netstat.InMcastPkts << " " 
            << netstat.OutMcastPkts << " " 
            << netstat.InBcastPkts << " " 
            << netstat.OutBcastPkts << " " 
            << netstat.InOctets << " " 
            << netstat.OutOctets << " " 
            << netstat.InMcastOctets << " " 
            << netstat.OutMcastOctets << " "
            << netstat.InBcastOctets << " "
            << netstat.OutBcastOctets;
        return os;
    }

    friend Netstat operator- (const Netstat &netstat1, const Netstat &netstat2) {
        Netstat netstat;
        netstat.InNoRoutes = netstat1.InNoRoutes - netstat2.InNoRoutes;
        netstat.InTruncatedPkts = netstat1.InTruncatedPkts - netstat2.InTruncatedPkts;
        netstat.InMcastPkts = netstat1.InMcastPkts - netstat2.InMcastPkts;
        netstat.OutMcastPkts = netstat1.OutMcastPkts - netstat2.OutMcastPkts;
        netstat.InBcastPkts = netstat1.InBcastPkts - netstat2.InBcastPkts;
        netstat.OutBcastPkts = netstat1.OutBcastPkts - netstat2.OutBcastPkts;
        netstat.InOctets = netstat1.InOctets - netstat2.InOctets;
        netstat.OutOctets = netstat1.OutOctets - netstat2.OutOctets;
        netstat.InMcastOctets = netstat1.InMcastOctets - netstat2.InMcastOctets;
        netstat.OutMcastOctets = netstat1.OutMcastOctets - netstat2.OutMcastOctets;
        netstat.InBcastOctets = netstat1.InBcastOctets - netstat2.InBcastOctets;
        netstat.OutBcastOctets = netstat1.OutBcastOctets - netstat2.OutBcastOctets;
        return netstat;
    }

    void readNetstat() {
        ifstream in(NETSTATFILE);
        string line;
        time_stamp = time(NULL);
        if(in) {
            getline(in, line);
            if(line.find("TcpExt") != 0) return;
            getline(in, line);
            if(line.find("TcpExt") != 0) return;
            getline(in, line);
            if(line.find("IpExt") != 0) return;
            getline(in, line);
            if(line.find("IpExt") != 0) return;
            vector<string> words = Util::getWords(line);
            InNoRoutes = stoull(words[1]);
            InTruncatedPkts = stoull(words[2]);
            InMcastPkts = stoull(words[3]);
            OutMcastPkts = stoull(words[4]);
            InBcastPkts = stoull(words[5]);
            OutBcastPkts = stoull(words[6]);
            InOctets = stoull(words[7]);
            OutOctets = stoull(words[8]);
            InMcastOctets = stoull(words[9]);
            OutMcastOctets = stoull(words[10]);
            InBcastOctets = stoull(words[11]);
            OutBcastOctets = stoull(words[12]);
        }
        in.close();
    }
};

#endif

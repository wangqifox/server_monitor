#ifndef NETSTAT_H
#define NETSTAT_H

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

class Netstat {
public:
    time_t time;
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

    Netstat():time(0),InNoRoutes(0),InTruncatedPkts(0),InMcastPkts(0),OutMcastPkts(0),InBcastPkts(0),OutBcastPkts(0),
                InOctets(0),OutOctets(0),InMcastOctets(0),OutMcastOctets(0),InBcastOctets(0),OutBcastOctets(0){}

    friend ostream& operator<< (ostream &os, Netstat &netstat);
    friend Netstat operator- (const Netstat &netstat1, const Netstat &netstat2);
};

#endif
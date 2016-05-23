#include <iostream>
#include <string>
#include <time.h>
#include "netstat.h"
using namespace std;

ostream& operator<< (ostream &os, Netstat &netstat) {
    os << netstat.time << " " 
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

Netstat operator- (const Netstat &netstat1, const Netstat &netstat2) {
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
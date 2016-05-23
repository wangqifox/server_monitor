#include <iostream>
#include <time.h>
#include "vmstat.h"

ostream& operator<< (ostream &os, Vmstat &vmstat) {
    os << "time: " << vmstat.time << " "
        << "pgpgin: " << vmstat.pgpgin << " "
        << "pgpgout: " << vmstat.pgpgout << " " 
        << "pswpin: " << vmstat.pswpin << " "
        << "pswpout: " << vmstat.pswpout;
    return os;
}

Vmstat operator- (const Vmstat &vmstat1, const Vmstat &vmstat2) {
    Vmstat vmstat;
    vmstat.pgpgin = vmstat1.pgpgin - vmstat2.pgpgin;
    vmstat.pgpgout = vmstat1.pgpgout - vmstat2.pgpgout;
    vmstat.pswpin = vmstat1.pswpin - vmstat2.pswpin;
    vmstat.pswpout = vmstat1.pswpout - vmstat2.pswpout;
    return vmstat;
}
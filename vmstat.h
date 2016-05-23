#ifndef VMSTAT_H
#define VMSTAT_H

#include <iostream>
#include <time.h>

using namespace std;

class Vmstat {
public:
    time_t time;
    unsigned long long pgpgin;
    unsigned long long pgpgout;
    unsigned long long pswpin;
    unsigned long long pswpout;

    Vmstat():time(0), pgpgin(0),pgpgout(0),pswpin(0),pswpout(0){}

    friend ostream& operator<< (ostream &os, Vmstat &vmstat);
    friend Vmstat operator- (const Vmstat &vmstat1, const Vmstat &vmstat2);
};

#endif

#ifndef PROGRESS_PERF_HPP
#define PROGRESS_PERF_HPP

#include <time.h>
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include "utils.hpp"

using namespace std;

class ProgressPerf {
private:
    const string CMDLINEFILE = "/proc/%d/cmdline";
    const string STATFILE = "/proc/%d/stat";
    string cmdlinefile;
    string statfile;

    time_t t;
    unsigned int pid;
    string cmdline;
    string task_state;
    unsigned long long utime;
    unsigned long long stime;
    unsigned long long rss;
    int task_cpu;

    void readCmdLine() {
        ifstream in(cmdlinefile);
        if(in) {
            string line;
            getline(in, line);
            replace(line.begin(), line.end(), '\0', ' ');
            cmdline = line;
        }
    }

    void readPerf() {
        ifstream in(statfile);
        if(in) {
            string line;
            getline(in, line);
            vector<string> words = Util::getWords(line);
            task_state = words[2];
            utime = stoull(words[13]);
            stime = stoull(words[14]);
            rss = stoull(words[23]);
            task_cpu = atoi(words[38].c_str());
        }
    }

public:
    ProgressPerf(unsigned int pid):pid(pid) {
        cmdlinefile = (boost::format(CMDLINEFILE) % pid).str();
        statfile = (boost::format(STATFILE) % pid).str();
    }

    void readData() {
        t = time(NULL);
        readCmdLine();
        readPerf();
    }

    time_t getTime() {
        return t;
    }

    unsigned int getPid() {
        return pid;
    }

    string getCmdLine() {
        return cmdline;
    }

    string getTaskState() {
        return task_state;
    }

    unsigned long long getutime() {
        return utime;
    }

    unsigned long long getstime() {
        return stime;
    }

    unsigned long long getrss() {
        return rss;
    }

    int getTaskCpu() {
        return task_cpu;
    }

    friend ostream& operator<< (ostream &os, ProgressPerf &progress_perf) {
        os << progress_perf.pid << " " 
            << progress_perf.t << " "
            << progress_perf.cmdline << " " 
            << progress_perf.task_state << " " 
            << progress_perf.utime << " " 
            << progress_perf.stime << " " 
            << progress_perf.rss << " "
            << progress_perf.task_cpu;
        return os;
    };

    unsigned long long operator- (ProgressPerf &progress_perf) {
        return getCputime() - progress_perf.getCputime();
    }

    unsigned long long getCputime() {
        return utime + stime;
    }
    
};

class ProgressRate {
public:
    unsigned int pid;
    double rate;
    string cmdline;
    string task_state;
    unsigned long long rss;
    int task_cpu;

    ProgressRate(ProgressPerf& progress_perf) {
        pid = progress_perf.getPid();
        cmdline = progress_perf.getCmdLine();
        task_state = progress_perf.getTaskState();
        rss = progress_perf.getrss();
        task_cpu = progress_perf.getTaskCpu();
    } 
};

class ProgressesPerf {
private:
    const string PROCDIR = "/proc";
    time_t t;
    map<unsigned int, ProgressPerf*> progresses_map;
    const string CPUFILE = "/proc/stat";

    unsigned long long cputime;

    unsigned long long readCputime() {
        unsigned long long user = 0;
        unsigned long long nice = 0;
        unsigned long long system = 0;
        unsigned long long idle = 0;
        unsigned long long iowait = 0;
        unsigned long long irq = 0;
        unsigned long long softirq = 0;
        unsigned long long steal = 0;
        unsigned long long guest = 0;
        unsigned long long guest_nice = 0;

        ifstream in(CPUFILE);
        string line;
        if(in) {
            getline(in, line);
            vector<string> words = Util::getWords(line);
            if(words[0].find("cpu") == 0) {
                user = stoull(words[1]);
                nice = stoull(words[2]);
                system = stoull(words[3]);
                idle = stoull(words[4]);
                iowait = stoull(words[5]);
                irq = stoull(words[6]);
                softirq = stoull(words[7]);
                steal = stoull(words[8]);

                if(words.size() >= 10) guest = stoull(words[9]);
                if(words.size() >= 11) guest_nice = stoull(words[10]);
            }
        }
        return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    }

public:
    ProgressesPerf() {}

    // ProgressesPerf(ProgressesPerf& progresses_perf) {
    // }

    virtual ~ProgressesPerf() {
        for (map<unsigned int, ProgressPerf*>::iterator it = progresses_map.begin(); it != progresses_map.end(); it++) {
            delete it->second;
        }
        progresses_map.clear();
    }

    time_t getTime() {
        return t;
    }

    void readStat() {
        // Util::listDir(PROCDIR, "\\d+");
        vector<string> dir = Util::listDir(PROCDIR, "\\d+");
        for (auto it = dir.begin(); it != dir.end(); it++) {
            unsigned int pid = atoi((*it).c_str());
            progresses_map[pid] = new ProgressPerf(pid);

            // vector<string> subdir = Util::listDir(tostring(PROCDIR, "/", pid, "/task"), "\\d+");
            // for (auto it = subdir.begin(); it != subdir.end(); it++) {
            //     // cout << *it << endl;
            //     unsigned int pid = atoi((*it).c_str());
            //     progresses_map[pid] = new ProgressPerf(pid);
            // }
            // subdir.clear();
        }
        dir.clear();

        t = time(NULL);
        cputime = readCputime();
        for (auto it = progresses_map.begin(); it != progresses_map.end(); it++) {
            ProgressPerf* progress_perf = it->second;
            progress_perf->readData();
        }
    }

    friend ostream& operator<< (ostream &os, ProgressesPerf &progresses_perf) {
        for (auto it = progresses_perf.progresses_map.begin(); it != progresses_perf.progresses_map.end(); it++) {
            cout << it->first << endl;
            cout << *(it->second) << endl;
        }
        return os;
    }

    map<unsigned int, ProgressRate> operator- (ProgressesPerf &progresses_perf) {
        map<unsigned int, ProgressRate> progresses_rate;
        unsigned long long cputime_diff = cputime - progresses_perf.cputime;
        for (auto it = progresses_map.begin(); it != progresses_map.end(); it++) {
            unsigned int pid = it->first;
            ProgressPerf* this_progress_perf = it->second;
            ProgressRate progress_rate(*this_progress_perf);

            if(progresses_perf.progresses_map.find(pid) != progresses_perf.progresses_map.end()) {
                unsigned long long progress_cputime_diff = *(this_progress_perf) - *(progresses_perf.progresses_map[pid]);
                progress_rate.rate = progress_cputime_diff * Util::getCpuCount() / (cputime_diff * 1.0);

                // if (progress_rate.cmdline.find("monitor") < progress_rate.cmdline.length()) {
                //     cout << this_progress_perf->getTime() << " " << progresses_perf.progresses_map[pid]->getTime() << endl;
                //     cout << this_progress_perf->getCputime() << " " << progresses_perf.progresses_map[pid]->getCputime() << endl;
                //     cout << progress_cputime_diff << " " << Util::getCpuCount() << " " << cputime_diff;
                // }
                
            } else {
                progress_rate.rate = this_progress_perf->getCputime() * Util::getCpuCount() / (cputime_diff * 1.0);
            }
            // if (progress_rate.cmdline.find("monitor") < progress_rate.cmdline.length()) {
            //     cout << progress_rate.pid << " " << progress_rate.cmdline << " " << progress_rate.rate << endl;
            // }
            progresses_rate.insert(pair<unsigned int, ProgressRate>(pid, progress_rate));
        }

        return progresses_rate;
    };
    
};

#endif
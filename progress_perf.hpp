#ifndef PROGRESS_PERF_HPP
#define PROGRESS_PERF_HPP

#include <time.h>
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <vector>
#include <map>
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
    unsigned long long task_cpu;

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
            // cout << line << endl;
            vector<string> words = Util::getWords(line);
            task_state = words[2];
            utime = stoull(words[13]);
            stime = stoull(words[14]);
            rss = stoull(words[23]);
            task_cpu = stoull(words[38]);
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

    string getCmdLine() {
        return cmdline;
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
    
};

class ProgressesPerf {
private:
    const string PROCDIR = "/proc";
    time_t t;
    map<unsigned int, ProgressPerf*> progresses_map;
    // vector<unsigned int> progress_ids;
public:
    ProgressesPerf() {}

    void readStat() {
        vector<string> dir = Util::listDir(PROCDIR, "\\d+");
        // cout << dir.size() << endl;
        for (auto it = dir.begin(); it != dir.end(); it++) {
            // cout << *it << endl;
            unsigned int pid = atoi((*it).c_str());
            // cout << pid << endl;
            progresses_map[pid] = new ProgressPerf(pid);
            // ProgressPerf progress_perf(12055);
            // progress_perf.readData();
        }

        t = time(NULL);
        for (auto it = progresses_map.begin(); it != progresses_map.end(); it++) {
            cout << it->first << endl;
            ProgressPerf* progress_perf = it->second;
            progress_perf->readData();
            cout << *(it->second) << endl;
        }
    }

    virtual ~ProgressesPerf() {
        for (auto it = progresses_map.begin(); it != progresses_map.end(); it++) {
            delete it->second;
        }
    }
};

#endif
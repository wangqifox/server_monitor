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

class Perf {
protected:
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
            try {
                string line;
                getline(in, line);
                replace(line.begin(), line.end(), '\0', ' ');
                cmdline = line;
            } catch (const std::exception& e) {
                cout << e.what() << endl;
            }
        }
    }

    void readPerf() {
        ifstream in(statfile);
        if(in) {
            try {
                string line;
                getline(in, line);
                vector<string> words = Util::getWords(line);
                task_state = words.at(2);
                utime = stoull(words.at(13));
                stime = stoull(words.at(14));
                rss = stoull(words.at(23));
                task_cpu = atoi(words.at(38).c_str());
            } catch (const std::exception& e) {
                cout << e.what() << endl;
            }
        }
    }
public:
    Perf(unsigned int pid):pid(pid){}

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

    unsigned long long getCputime() {
        return utime + stime;
    }

    friend ostream& operator<< (ostream &os, Perf &progress_perf) {
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

    unsigned long long operator- (Perf &progress_perf) {
        return getCputime() - progress_perf.getCputime();
    }

     virtual ~Perf() {

     }

};

class TaskPerf : public Perf {
private:
    const string CMDLINEFILE = "/proc/%d/task/%d/cmdline";
    const string STATFILE = "/proc/%d/task/%d/stat";
    unsigned int tid;

public:
    TaskPerf(unsigned int pid, unsigned int tid):Perf(pid), tid(tid) {
        cmdlinefile = (boost::format(CMDLINEFILE) % pid % tid).str();
        statfile = (boost::format(STATFILE) % pid % tid).str();
    }

    unsigned int getTid() {
        return tid;
    }
};

class ProgressPerf : public Perf {
private:
    const string CMDLINEFILE = "/proc/%d/cmdline";
    const string STATFILE = "/proc/%d/stat";
public:
    map<unsigned int, TaskPerf*> threads;

    ProgressPerf(unsigned int pid):Perf(pid) {
        cmdlinefile = (boost::format(CMDLINEFILE) % pid).str();
        statfile = (boost::format(STATFILE) % pid).str();
    }

    void addThread(unsigned int tid, TaskPerf* task_perf) {
        threads.insert(pair<unsigned int, TaskPerf*>(tid, task_perf));
        // threads.push_back(task_perf);
    }

    virtual ~ProgressPerf() {
        for (auto it = threads.begin(); it != threads.end(); it++) {
            delete it->second;
        }
        threads.clear();
    }

    void readData() {
        Perf::readData();
        for(auto it = threads.begin(); it != threads.end(); it++) {
            it->second->readData();
        }
    }

};

class Rate {
public:
    unsigned int pid;
    double rate;
    string cmdline;
    string task_state;
    unsigned long long rss;
    int task_cpu;

    Rate(Perf* perf) {
        pid = perf->getPid();
        cmdline = perf->getCmdLine();
        task_state = perf->getTaskState();
        rss = perf->getrss();
        task_cpu = perf->getTaskCpu();
    }
};

class ThreadRate: public Rate {
public:
    unsigned int tid;
    ThreadRate(TaskPerf* task_perf): Rate(task_perf) {
        tid = task_perf->getTid();
    }

};

class ProgressRate : public Rate{
public:
    vector<ThreadRate> thread_rates;

    ProgressRate(ProgressPerf* progress_perf): Rate(progress_perf) {}

    // virtual ~ProgressRate() {
    //     for (auto it = thread_rates.begin(); it != thread_rates.end(); it++) {
    //         delete *it;
    //     }
    //     thread_rates.clear();
    // }
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
            try {
                getline(in, line);
                vector<string> words = Util::getWords(line);
                if(words.at(0).find("cpu") == 0) {
                    user = stoull(words.at(1));
                    nice = stoull(words.at(2));
                    system = stoull(words.at(3));
                    idle = stoull(words.at(4));
                    iowait = stoull(words.at(5));
                    irq = stoull(words.at(6));
                    softirq = stoull(words.at(7));
                    steal = stoull(words.at(8));

                    if(words.size() >= 10) guest = stoull(words.at(9));
                    if(words.size() >= 11) guest_nice = stoull(words.at(10));
                }
            } catch (const std::exception& e) {
                cout << e.what() << endl;
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
            ProgressPerf* progress_perf = new ProgressPerf(pid);

            vector<string> subdir = Util::listDir(tostring(PROCDIR, "/", pid, "/task"), "\\d+");
            for (auto it = subdir.begin(); it != subdir.end(); it++) {
                // cout << *it << endl;
                unsigned int tid = atoi((*it).c_str());
                TaskPerf* task_perf = new TaskPerf(pid, tid);
                progress_perf->addThread(tid, task_perf);
                // progresses_map[pid] = new ProgressPerf(pid);
            }

            progresses_map[pid] = progress_perf;

            subdir.clear();
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
            ProgressRate progress_rate(this_progress_perf);

            if(progresses_perf.progresses_map.find(pid) != progresses_perf.progresses_map.end()) {
                ProgressPerf* other_progress_perf = progresses_perf.progresses_map[pid];
                unsigned long long progress_cputime_diff = *(this_progress_perf) - *(other_progress_perf);
                progress_rate.rate = progress_cputime_diff * Util::getCpuCount() / (cputime_diff * 1.0);

                map<unsigned int, TaskPerf*> this_progress_theads = this_progress_perf->threads;
                map<unsigned int, TaskPerf*> other_progress_theads = other_progress_perf->threads;
                for (auto it2 = this_progress_theads.begin(); it2 != this_progress_theads.end(); it2++) {
                    unsigned int tid = it2->first;
                    TaskPerf* this_thread_perf = it2->second;
                    ThreadRate thread_rate(this_thread_perf);
                    if(other_progress_theads.find(tid) != other_progress_theads.end()) {
                        TaskPerf* other_thread_perf = other_progress_theads[tid];
                        unsigned long long thread_cputime_diff = *(this_thread_perf) - *(other_thread_perf);
                        thread_rate.rate = thread_cputime_diff * Util::getCpuCount() / (cputime_diff * 1.0);
                    } else {
                        thread_rate.rate = this_thread_perf->getCputime() * Util::getCpuCount() / (cputime_diff * 1.0);
                    }
                    progress_rate.thread_rates.push_back(thread_rate);
                }
                
            } else {
                progress_rate.rate = this_progress_perf->getCputime() * Util::getCpuCount() / (cputime_diff * 1.0);
            }

            progresses_rate.insert(pair<unsigned int, ProgressRate>(pid, progress_rate));
        }

        return progresses_rate;
    };
    
};

#endif
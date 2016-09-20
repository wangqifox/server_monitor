#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <dirent.h>
#include <string>

// #include <boost/format.hpp>
// #include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

using namespace std;


class Util {
// private:
//     // const string CPUFILE = "/proc/stat";


//     const static int cpuCount = getCpuCount();
    

    

public:
    static vector<string> getWords(string line) {
        vector<string> words;
        int start = 0, end = 1;
        while(end > 0) {
            end = line.find_first_of(" ", start);
            string str = line.substr(start, end-start);
            words.push_back(str);
            start = line.find_first_not_of(" ", end);
        }
        
        return words;
    }

    static string runCommand(const char* cmd) {
        FILE *in;
        char buff[512];
        string result;
        if(!(in = popen(cmd, "r"))) {
            return NULL;
        }
        while(fgets(buff, sizeof(buff), in) != NULL) {
            result.append(buff);
        }
        pclose(in);
        return result;
    }

    static double doubleSetPrecision(double num, int precision) {
        stringstream ss;
        ss << fixed << setprecision(precision) << num;
        ss >> num;
        return num;
    }

    static string doubleTostring(double num, int precision) {
        stringstream ss;
        ss << fixed << setprecision(precision) << num;
        return ss.str();
    }

    static int getPageSize() {
        string PAGE_SIZE = runCommand("getconf PAGE_SIZE");
        return stoul(PAGE_SIZE);
    }

    static vector<string> listDir(string path, const string pattern) {
        struct dirent *ptr;
        vector<string> files;
        DIR *dir;
        if ((dir = opendir(path.c_str())) != NULL) {
            boost::regex expression(pattern);
            while((ptr=readdir(dir)) != NULL) {
                // if(ptr->d_name[0] == '.')
                //  continue;
                boost::cmatch what;
                if(boost::regex_match(ptr->d_name, what, expression))
                    files.push_back(ptr->d_name);
            }
            closedir(dir);
        }       
        return files;
    }

    static int getCpuCount() {
        static int cpuCount = 0;
        if(cpuCount == 0) {
            ifstream in("/proc/stat");
            string line;
            int count = -1;
            if(in) {
                while(getline(in, line)) {
                    if(line.find("cpu") ==0)
                        count++;
                }
            }
            cpuCount = count;
        }
        
        return cpuCount;
    }

    
};

template<typename T>
string tostring(T t){
    stringstream ss;
    ss << t;
    return ss.str();
}

template<typename T, typename... Params>
string tostring(const T& t, Params... params)  
{  
    return tostring(t) + tostring(params...);
}

#endif

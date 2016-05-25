#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>

// #include <boost/format.hpp>
// #include <boost/algorithm/string.hpp>
// #include <boost/algorithm/string/regex.hpp> 
#include "utils.h"

using namespace std;

// vector<string> getWords(string line){
//     vector<string> words;
//     boost::algorithm::split_regex(words, line, boost::regex("\\s+"));
//     return words;
// }

vector<string> getWords(string line) {
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

string runCommand(const char* cmd) {
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

double doubleSetPrecision(double num, int precision) {
    stringstream ss;
    ss << fixed << setprecision(precision) << num;
    ss >> num;
    return num;
}

string doubleTostring(double num, int precision) {
    stringstream ss;
    ss << fixed << setprecision(precision) << num;
    return ss.str();
}

int getPageSize() {
    string PAGE_SIZE = runCommand("getconf PAGE_SIZE");
    return stoul(PAGE_SIZE);
}
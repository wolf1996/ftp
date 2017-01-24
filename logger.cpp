#include "logger.h"

boost::filesystem::path Logger::logfile("log");
std::ofstream Logger::filedescr;
bool Logger::inited = false;
std::mutex Logger::locker;

Logger& Logger::Instance() {
    static Logger s;
    return s;
}

Logger::Logger(){

}

Logger::~Logger(){
    filedescr.close();
}

int  Logger::InitLog(std::string logpath){
    std::lock_guard<std::mutex> lock(locker);
    if (inited) {
        return 1;
    }
    boost::filesystem::path bpath(logpath);
    logfile = bpath;
    filedescr.open(logpath);
    return 0;
}


int  Logger::InitLog(){
    std::lock_guard<std::mutex> lock(locker);
    if (inited) {
        return 1;
    }
    filedescr.open(logfile.string());
    return 0;
}



void Logger::Debug(const std::string& msg){
    std::lock_guard<std::mutex> lock(locker);
    filedescr << "DEBUG \t" << msg << std::endl;
    filedescr.flush();
}

void Logger::Log(const std::string& msg){
    std::lock_guard<std::mutex> lock(locker);
    filedescr << "LOG \t" << msg << std::endl;
}

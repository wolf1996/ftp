#include "logger.h"

boost::filesystem::path Logger::logfile("log");
std::ofstream Logger::filedescr;

Logger& Logger::Instance() {
    static Logger s;
    return s;
}

Logger::Logger(){

}

Logger::~Logger(){
    filedescr.close();
}

void  Logger::InitLog(std::string logpath){
    boost::filesystem::path bpath(logpath);
    logfile = bpath;
    filedescr.open(logpath);
}


void  Logger::InitLog(){
    filedescr.open(logfile.string());
}



void Logger::Debug(const std::string& msg){
    filedescr << "DEBUG \t" << msg << std::endl;
    filedescr.flush();
}

void Logger::Log(const std::string& msg){
    filedescr << "LOG \t" << msg << std::endl;
}

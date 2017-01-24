#ifndef LOGGER_H
#define LOGGER_H

#include <boost/filesystem/path.hpp>
#include <fstream>
#include <mutex>


class Logger
{
  public:
    static Logger& Instance();
    static int InitLog(std::string logpath);
    static int InitLog();
    void Debug(const std::string&);
    void Log(const std::string& );
  private:
    static std::mutex locker;
    static boost::filesystem::path logfile;
    static std::ofstream filedescr;
    static bool inited;
    Logger();
    ~Logger();
    Logger(Logger const&); // реализация не нужна
    Logger& operator= (Logger const&);  // и тут
};

#endif // LOGGER_H

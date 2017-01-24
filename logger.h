#ifndef LOGGER_H
#define LOGGER_H

#include <boost/filesystem/path.hpp>
#include <fstream>

class Logger
{
  public:
    static Logger& Instance();
    static void InitLog(std::string logpath);
    static void InitLog();
    void Debug(const std::string&);
    void Log(const std::string& );
  private:
    static boost::filesystem::path logfile;
    static std::ofstream filedescr;
    Logger();
    ~Logger();
    Logger(Logger const&); // реализация не нужна
    Logger& operator= (Logger const&);  // и тут
};

#endif // LOGGER_H

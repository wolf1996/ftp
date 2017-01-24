#ifndef CLIENT_H
#define CLIENT_H
#include <sstream>
#include <regex>
#include <string>
#include <fstream>
#include <memory>
#include <future>
#include <iterator>
#include <regex>
#include <iostream>
#include <string>
#include <regex>
#include <chrono>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "myserversocket.h"
#include "filesystem.h"
#include "tcpconn.h"

class Client
{
    enum state {ASCII, BINARY};
    state st;
    std::shared_ptr<MyServerSocket> sock;
    FileSystem fs;
    bool _finish;
    std::future<int> fut;
    std::shared_ptr<MyServerSocket> datasock;
    std::string sendfile(std::shared_ptr<std::fstream>& results);
    std::string storefile(std::shared_ptr<std::fstream>& results);
    void createDatasock();
    void deleteDatasock();
    void store(std::string &);
    void pwd(std::string &);
    void list(std::string &);
    void port(std::string &);
    void user(std::string &);
    void pass(std::string &);
    void syst(std::string &);
    void cwd(std::string &);
    void mkd(std::string & strres);
    void rmd(std::string & strres);
    void retr(std::string & strres);
    void type(std::string & strres);
    void finish_translation();
    void accept();
    void quit(std::string & strres);
public:
    Client(std::shared_ptr<MyServerSocket> _sock,
           boost::filesystem::path _wdir);
    //Client(const Client&);
    void execCmd();
    bool finish();
    bool hasCmd();
};

#endif // CLIENT_H

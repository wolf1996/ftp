#include <streambuf>
#include <iostream>
#include <sstream>
#include "boost/filesystem/path.hpp"
#include <myserversocket.h>
#include <algorithm>
#include <vector>

#include "client.h"
#include "logger.h"

int main(int argc, char *argv[])
{
#if _WIN32
    boost::filesystem::path pt("C:\ftp\test");
#elif __linux__
    boost::filesystem::path pt("/home/ksg/disk_d/labs_2016_4k/ramus");
#endif
    Logger::InitLog(std::string("mylogfile"));
    Logger& logs = Logger::Instance();
    logs.Debug("WorkS!");
    MyServerSocket sock;
    sock.mbind(12345);
    sock.mlisten();
    std::vector<Client*> clients;
        while (1) {
            usleep(100);
            while(std::shared_ptr<MyServerSocket> client = sock.maccept())
            {
                client->setNonBlocked(1);
                Client* buf = new Client(client,pt);
                clients.push_back(buf);
            }
            for(std::vector<Client*>::iterator i = clients.begin(); i < clients.end(); i++) {
                if(((*i)->hasCmd())&&(!(*i)->finish())) {
                    (*i)->execCmd();
                }
            }
            for(std::vector<Client*>::iterator i = clients.begin(); i < clients.end(); i++) {
                if((*i)->finish()) {
                    Client* buf = *i;
                    clients.erase(i);
                    delete buf;
                }
            }
        }
    return 0;
}

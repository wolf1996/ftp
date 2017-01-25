#include <streambuf>
#include <iostream>
#include <sstream>
#include "boost/filesystem/path.hpp"
#include <myserversocket.h>
#include <algorithm>
#include <vector>

#include "client.h"
#include "logger.h"
#include "parser_arg.h"

int main(int argc, char *argv[])
{
    parser_arg flags(argc,argv);
    boost::filesystem::path pt(flags.get_work_dir());

    Logger::InitLog(std::string("mylogfile"));
    Logger& logs = Logger::Instance();
    logs.Log("Starting server");
    MyServerSocket sock;
    sock.mbind(flags.get_port());
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

#ifndef MYCLIENTSOCKET_H
#define MYCLIENTSOCKET_H

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif _WIN32
#include <win_include.h>
#endif


#include <thread>
#include <vector>
#include "mytcpsocket.h"

class MyClientSocket : public MyTCPSocket
{
    MyTCPSocket sock;
public:
    void mconnect(char ip[4], int port);
    //void mconnect(char ip[4], int port, unsigned long timeout);
    void msend(const std::vector<char> & msg);
    MyClientSocket();
    ~MyClientSocket();
};

#endif // MYCLIENTSOCKET_H

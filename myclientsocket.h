#ifndef MYCLIENTSOCKET_H
#define MYCLIENTSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

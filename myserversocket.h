#ifndef MYSERVERSOCKET_H
#define MYSERVERSOCKET_H

#ifdef __linux__
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#elif _WIN32
#include <win_include.h>
#endif

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>


#include "mytcpsocket.h"

class MyServerSocket : public MyTCPSocket
{
#if _WIN32
    WSADATA wsadata;
#endif
    MyTCPSocket sock;
public:
    MyServerSocket();
    MyServerSocket(int sock):MyTCPSocket(sock){}
    void mbind( int port);
    void mlisten();
    std::shared_ptr<MyServerSocket> maccept();
    std::vector<char> mrecv();
    std::vector<char> mrecv(int &n);
    void msend(const std::vector<char> & msg);
    bool hasData();
    void mconnect(char ip[4], int port);
    void mdisconnect();
};

#endif // MYSERVERSOCKET_H

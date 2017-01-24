#ifndef MYSERVERSOCKET_H
#define MYSERVERSOCKET_H

#include <sys/ioctl.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mytcpsocket.h"

class MyServerSocket : public MyTCPSocket
{
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

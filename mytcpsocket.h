#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdexcept>

class MyTCPSocket
{
public:
    int sDescr;
public:
    MyTCPSocket(): sDescr(-1){}
    MyTCPSocket(int descr): sDescr(descr){}
    virtual void initSocket();
    virtual void setNonBlocked(bool opt);
    virtual ~MyTCPSocket();
};

#endif // MYTCPSOCKET_H

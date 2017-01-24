#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#elif _WIN32
#include <winsock2.h>
#include <windows.h>
#include <errno.h>
#endif

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdexcept>

class MyTCPSocket
{
public:
#ifdef __linux__
    int sDescr;
#elif _WIN32
    SOCKET sDescr;
#endif
public:
    MyTCPSocket(): sDescr(-1){}
    MyTCPSocket(int descr): sDescr(descr){}
    virtual void initSocket();
    virtual void setNonBlocked(bool opt);
    virtual ~MyTCPSocket();
};

#endif // MYTCPSOCKET_H

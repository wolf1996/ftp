#include "mytcpsocket.h"

void MyTCPSocket::initSocket()
{
    if(sDescr != -1) {
        throw std::runtime_error("already inited");
    }
    sDescr = socket(PF_INET, SOCK_STREAM,  IPPROTO_TCP);
    if(sDescr < 0) {
        throw std::runtime_error(std::string(strerror(errno)));
    }
}

MyTCPSocket::~MyTCPSocket() {
    if(sDescr > 0) {
        close(sDescr);
    }
}

void MyTCPSocket::setNonBlocked(bool opt) {
#ifdef __linux__
    int flags = fcntl(sDescr, F_GETFL, 0);
    int new_flags = (opt)? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    if (fcntl(sDescr, F_SETFL, new_flags) == -1)
        throw std::runtime_error("make nonblocked: " + std::string(strerror(errno)));
#elif _WIN32
    if (SOCKET_ERROR == ioctlsocket (sDescr, FIONBIO, (unsigned long* ) &opt) )
    {
        throw std::runtime_error("make nonblocked, error number: " + std::to_string(WSAGetLastError()));
    }
#endif
}

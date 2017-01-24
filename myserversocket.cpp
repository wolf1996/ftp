#include "myserversocket.h"

#define BUFSIZE 256
#define TIME_OUT 100000

MyServerSocket::MyServerSocket()
{

}

void MyServerSocket::mbind(int port)
{
    MyTCPSocket::initSocket();
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(struct sockaddr_in));
    int on = 1;
    if(setsockopt(sDescr,SOL_SOCKET,SO_REUSEADDR,(char*)(&on),sizeof(on)) < 0) {
        throw std::runtime_error("set rcvtimeout: " + std::string(strerror(errno)));
    }
    if(ioctl(sDescr, FIONBIO, (char*)&on)) {
        throw std::runtime_error(std::string("setsockopt failed"));
    }
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);
    if(bind(sDescr, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error(std::string("bind failed"));
    }
}


bool MyServerSocket::hasData() {
    char buf[1];
    int n = recv(sDescr,buf,sizeof(buf),MSG_PEEK);
    return n > 0;
}

void MyServerSocket::mlisten()
{
    if(listen(sDescr, 64) < 0) {
        throw std::runtime_error(std::string("listen failed"));
    }
}

std::shared_ptr<MyServerSocket> MyServerSocket::maccept() {
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    socklen_t cli_len = sizeof(client);
    int cli_sd = accept(sDescr, (struct sockaddr*)& client, &cli_len);
    if (-1 == cli_sd)
        return std::shared_ptr<MyServerSocket>();
    std::cerr << "new client: " << cli_sd << ", from: "<<client.sin_addr.s_addr << std::endl;
    return std::make_shared<MyServerSocket>(cli_sd);
}

std::vector<char> MyServerSocket::mrecv()
{
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    int n = recv(sDescr, buf, BUFSIZE, 0);
    if (-1 == n && errno != EAGAIN)
        throw std::runtime_error("read failed: " + std::string(strerror(errno)));
    if (0 == n)
        throw std::runtime_error("client: " + std::to_string(sDescr) + " disconnected");
    if (-1 == n)
        throw std::runtime_error("client: " + std::to_string(sDescr) + " timeouted");
    std::vector<char> ret(buf, buf+n);
    std::cout << "client: " << sDescr << ", recv: " << n << " [" << n << " bytes]" << std::endl;
    return ret;
}

std::vector<char> MyServerSocket::mrecv(int &n)
{
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    n = recv(sDescr, buf, BUFSIZE, 0);
    if (-1 == n && errno != EAGAIN)
        throw std::runtime_error("read failed: " + std::string(strerror(errno)));
    if (-1 == n)
        throw std::runtime_error("client: " + std::to_string(sDescr) + " timeouted");
    std::vector<char> ret(buf, buf+n);
    std::cout << "client: " << sDescr << ", recv: " << n << " [" << n << " bytes]" << std::endl;
    return ret;
}


void MyServerSocket::msend(const std::vector<char> & msg) {
    size_t size = msg.size();
    char buf[BUFSIZE];
    for(unsigned int i = 0; i < size; i+= BUFSIZE) {
        memset(buf,0,BUFSIZE);
        for(unsigned int j = 0; (j<BUFSIZE) && (j<size); j++) {
            buf[j] = msg[i+j];
        }
        if(send(sDescr,buf,((size - i) > BUFSIZE)?BUFSIZE:(size-i), 0) == -1) {
            throw std::runtime_error(std::string("send_error"+std::string(strerror(errno))));
        }
    }
}

 void MyServerSocket:: mconnect(char ip[], int port) {
     initSocket();
     struct sockaddr_in addr;
     memset(&addr,0,sizeof(struct sockaddr_in));
     addr.sin_family = AF_INET;
     addr.sin_port = htons(port);
     addr.sin_addr.s_addr = htonl(INADDR_ANY);
     if(inet_aton(ip, &addr.sin_addr) == 0) {
         throw std::runtime_error(std::string("inet aton error"));
     }
     int rc = connect(sDescr, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in));
     if( rc == -1 ) {
         throw std::runtime_error("connect error: " + std::string(strerror(errno)));
     }
 }

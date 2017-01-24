#include "myclientsocket.h"

#define BUFSIZE 256

MyClientSocket::MyClientSocket()
{

}
MyClientSocket::~MyClientSocket()
{

}

 void MyClientSocket::mconnect(char ip[], int port) {
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

 /*void MyClientSocket::mconnect(char ip[4], int port, unsigned long timeout) {
     struct sockaddr_in addr;
     memset(&addr,0,sizeof(struct sockaddr_in));
     addr.sin_family = AF_INET;
     addr.sin_port = htons(port);
     addr.sin_addr.s_addr = htonl(INADDR_ANY);
     set_non_blocked_impl(sDescr, true);
     if(inet_aton(ip, &addr.sin_addr) == 0) {
         std::runtime_error(std::string("inet aton error"));
     }
     int rc = connect(sDeskr, &addr, sizeof(struct sockaddr_in));
     if( rc == -1 && errno != EINPROGRESS ) {
         std::runtime_error(std::string("connect error"));
     }


 }*/

 void MyClientSocket::msend(const std::vector<char> & msg) {
     size_t size = msg.size();
     char buf[BUFSIZE];
     for(unsigned int i = 0; i < size; i+= BUFSIZE) {
         memset(buf,0,BUFSIZE);
         for(unsigned int j = 0; (j<BUFSIZE) && (j<size); j++) {
             buf[j] = msg[i+j];
         }
         if(send(sDescr,buf,BUFSIZE, 0) == -1) {
             throw std::runtime_error(std::string("send_error"));
         }
     }
 }

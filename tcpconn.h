#ifndef TCPCONN_H
#define TCPCONN_H

#include <vector>
#include <iostream>
#include <future>
#include <myserversocket.h>
#include <fstream>

int tcpConR(std::fstream& des, int port);
int tcpConS(std::shared_ptr<MyServerSocket> sock, std::vector<char> des);
int tcpConSFileB(std::shared_ptr<MyServerSocket> sock,  std::shared_ptr<std::fstream>);
int tcpConSFileA(std::shared_ptr<MyServerSocket> sock,  std::shared_ptr<std::fstream>);
int tcpConGFileB(std::shared_ptr<MyServerSocket> sock,  std::shared_ptr<std::fstream>);
int tcpConGFileA(std::shared_ptr<MyServerSocket> sock,  std::shared_ptr<std::fstream>);

#endif // TCPCONN_H

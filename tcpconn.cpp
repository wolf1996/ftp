#include "tcpconn.h"

int tcpConR(std::fstream& des, int port){
    MyServerSocket sock;
    sock.mbind(port);
    sock.mlisten();

    return 0;
}

int tcpConS(std::shared_ptr<MyServerSocket> sock, std::vector<char> des){
    try {
        sock->msend(des);
    }catch(std::exception){
        return 1;
    }

    return 0;
}

int tcpConSFileB(std::shared_ptr<MyServerSocket> sock, std::shared_ptr<std::fstream> file){
    std::vector<char> data;
    if(!file->is_open()){
        return 1;
    }
    while(!file->eof()){
        char symb;
        file->read(&symb,1);
        data.push_back(symb);
        if(symb == 0xFF){
            data.push_back(symb);
        }

           if(data.size() >= 2048){
              try{
                   sock->msend(data);
                   data.clear();
              } catch(std::exception& e){
                 return 1;
              }
            }

    }
    data.push_back(0xFF);
    data.push_back(2);
    try{
         sock->msend(data);
    } catch(std::exception& e){
       return 1;
    }
    std::cout << "end file translation" << std::endl;
    return 0;
}

int tcpConSFileA(std::shared_ptr<MyServerSocket> sock, std::shared_ptr<std::fstream> file){
    std::vector<char> data;
    if(!file->is_open()){
        return 1;
    }
    while(!file->eof()){
        char symb;
        file->read(&symb,1);
        data.push_back(symb);
        if(data.size() >= 2048){
           try{
                   sock->msend(data);
                   data.clear();
           } catch(...){
                 return 1;
           }
        }

    }
    try{
         sock->msend(data);
    } catch(...){
       return 1;
    }
    std::cout << "end file translation" << std::endl;
    return 0;
}

int tcpConGFileA(std::shared_ptr<MyServerSocket> sock, std::shared_ptr<std::fstream> file){
    if(!file->is_open()){
        return 1;
    }
    int n = 1;
    while(n){
        std::vector<char> data = sock->mrecv(n);
        for(unsigned int i =0 ; i < data.size(); i++){
            file->write(&data[i],1);
        }

    }
    std::cout << "end file translation" << std::endl;
    return 0;
}


int tcpConGFileB(std::shared_ptr<MyServerSocket> sock, std::shared_ptr<std::fstream> file){
    std::vector<char> data;
    if(!file->is_open()){
        return 1;
    }
    int n = 1;
    while(n){
        std::vector<char> data = sock->mrecv(n);
        for(unsigned int i =0 ; i < data.size(); i++){
            //cout << data[i];
            if(data[i] == 0xFF){
                if(data[i+1] == 0xFF){
                    i++;
                } else {
                    n = 0;
                    break;
                }
            }
            file->write(&data[i],1);
        }
    }
    std::cout << "end file translation" << std::endl;
    return 0;
}


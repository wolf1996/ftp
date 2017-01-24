#include "fileowner.h"

#define BIG_BLOCKSIZE 1024

FileOwner::FileOwner(std::shared_ptr<MyServerSocket> _sock): sock(std::move(_sock)), fdescr(0), filename(""), size(0), pos(0),  _finish(0)
{
    fdescr = std::shared_ptr<std::fstream>(new std::fstream);
}

FileOwner::FileOwner(const FileOwner& _f): sock(_f.sock),fdescr(_f.fdescr), filename(_f.filename),size(_f.size), pos(_f.pos), _finish(_f._finish)
{

}

bool FileOwner::finish() {
    return _finish;
}

void FileOwner::getHeader() {
    std::vector<char> data = sock->mrecv();
    std::vector<char>::iterator i;
    for(i= data.begin(); (i != data.end()) && (*i); i++) {
        filename.push_back(*i);
    }
    std::string buf;
    i++;
    for(;(i < data.end()) && (*i);i++) {
        if (!isdigit(*i)) {
            throw std::runtime_error("size format error");
        }
        buf.push_back(*i);
    }
    size = std::stoull(buf);
    //fdescr = std::fstream(filename,std::ios::out|std::ios::binary);
    fdescr->open(filename,std::fstream::out|std::ios::binary);
}

void FileOwner::getData() {
    for(unsigned long long int i = 0; i < size;) {
        std::vector<char> dt = sock->mrecv();
        if(size > (i + dt.size())) {
            fdescr->write(dt.data(),dt.size());
        } else {
            fdescr->write(dt.data(), size - i);
        }
        i += dt.size();
    }
    fdescr->flush();
}

bool FileOwner::hasData() {
    return sock->hasData();
}

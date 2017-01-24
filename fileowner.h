#ifndef FILEOWNER_H
#define FILEOWNER_H
#include <string>
#include <fstream>
#include <memory>
#include <boost/filesystem/path.hpp>
#include "myserversocket.h"

class FileOwner
{
    std::shared_ptr<MyServerSocket> sock;
    std::shared_ptr<std::fstream> fdescr;
    std::string filename;
    unsigned long long size;
    unsigned long long pos;
    bool _finish;
public:
    FileOwner(std::shared_ptr<MyServerSocket> _sock);
    FileOwner(const FileOwner&);
    void getHeader();
    void getData();
    bool finish();
    bool hasData();
};

#endif // FILEOWNER_H

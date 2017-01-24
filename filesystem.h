#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include  <string>
#include  "boost/filesystem.hpp"
#include  <fstream>

class FileSystem
{
    boost::filesystem::path basepath;
    boost::filesystem::path current;
    boost::filesystem::path getpath(std::string& inp);
public:
    FileSystem(boost::filesystem::path basepath);
    std::string pwd();
    std::string ls(std::string &dir);
    void cd(std::string &dir);
    void mkdir(std::string &dir);
    unsigned int  rmdir(std::string &dir);
    std::shared_ptr<std::fstream> file(std::string &dir);
    std::shared_ptr<std::fstream> newfile(std::string &dir);
};

#endif // FILESYSTEM_H

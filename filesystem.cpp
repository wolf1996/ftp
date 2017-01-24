#include "filesystem.h"

FileSystem::FileSystem(boost::filesystem::path _basepath):
basepath(_basepath), current("/")
{
}


boost::filesystem::path FileSystem::getpath(std::string& askPath) {
    boost::filesystem::path path;
    if(askPath[0] == '/'){
        path = basepath / boost::filesystem::path(askPath);
    } else {
        path = basepath /current / boost::filesystem::path(askPath);
    }
    return path;
}

std::__cxx11::string FileSystem::pwd(){
    return current.string();
}


std::string FileSystem::ls( std::string& askPath){
    boost::filesystem::path path = getpath(askPath);
    if (!boost::filesystem::is_directory(path)){
        throw std::runtime_error("It is not directory");
    }
    boost::filesystem::directory_iterator files(path);
    std::string res;
    while(files != boost::filesystem::directory_iterator()){
        res += files->path().filename().string();
        res += "\r\n";
        files ++;
    }
    return res;
}

void FileSystem::cd(std::string &askPath){
    boost::filesystem::path path = getpath(askPath);
    if (!boost::filesystem::is_directory(path)){
        throw std::runtime_error("It is not directory");
    }
    if(askPath[0] == '/'){
        current = boost::filesystem::path(askPath);
    } else {
        current /= boost::filesystem::path(askPath);
    }

}

void FileSystem::mkdir(std::string &askPath){
    boost::filesystem::path path = getpath(askPath);
    if(!boost::filesystem::create_directory(path)){
        throw std::runtime_error("error");
    }
}

unsigned int FileSystem::rmdir(std::string &askPath){
    boost::filesystem::path path = getpath(askPath);
    unsigned int num = 0;
    try{
        num = boost::filesystem::remove_all(path);

    }catch(std::exception& e){
        std::runtime_error(e.what());
    }
    return num;
}

std::shared_ptr<std::fstream> FileSystem::file(std::string &askPath){
    boost::filesystem::path path = getpath(askPath);
    if (!boost::filesystem::is_regular_file(path)){
        throw std::runtime_error("no file no problem");
    }
    std::shared_ptr<std::fstream> result(new std::fstream(path.string(),std::ios::binary|std::ios::in|std::ios::out));
    if(!result->is_open()){
        throw std::runtime_error("file open problem");
    }
    return result;
}

std::shared_ptr<std::fstream> FileSystem::newfile(std::string &askPath){
    boost::filesystem::path path = getpath(askPath);
    std::shared_ptr<std::fstream> result(new std::fstream(path.string(),std::ios::binary|std::ios::out));
    if(!result->is_open()){
        throw std::runtime_error("file open problem");
    }
    return result;
}

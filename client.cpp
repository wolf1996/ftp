#include "client.h"

#define BIG_BLOCKSIZE 1024

// TODO
// uploading
// delete sessions

Client::Client(std::shared_ptr<MyServerSocket> _sock,
               boost::filesystem::path _wdir):
    sock(std::move(_sock)), _finish(0),fs(_wdir), st(ASCII)
{
    datasock = 0;
    accept();
}

void Client::finish_translation(){
    int res = fut.get();
    Logger& logs = Logger::Instance();
    logs.Log("translation finished \n");
    deleteDatasock();
    if(res){
         char msg[]= "501 send failed \r\n";
         logs.Log("translation failed\n");
         std::vector<char> cmd(msg,msg + sizeof(msg));
         (*sock).msend(cmd);
    } else {
        char msg[]= "226 send,ok\r\n";
        logs.Log("translation ok\n");
        std::vector<char> cmd(msg,msg + sizeof(msg));
        (*sock).msend(cmd);
    }

}

void Client::pwd(std::string & strres){
    std::cout << "pwd gotten" << std::endl;
    std::string msg = boost::str(boost::format("257 \"%s\"\r\n")%fs.pwd());
    std::vector<char> cmd(msg.begin(),msg.end());
    (*sock).msend(cmd);
}

void Client::store(std::string & strres){
    std::cout << "store gotten" << std::endl;
    std::string inpPath(strres.begin()+ 4, strres.end());
    boost::trim_left(inpPath);
    boost::trim_right(inpPath);
    std::shared_ptr<std::fstream> results;
    try{
        results = fs.newfile(inpPath);
    } catch(std::exception& e){
        std::cout << e.what() << std::endl;
        char msg[] = "501\r\n";
        std::vector<char> cmd(msg,msg+sizeof(msg));
        (*sock).msend(cmd);
        return;
    }
    std::string msg = storefile(results);
    std::vector<char> cmd(msg.begin(),msg.end());
    (*sock).msend(cmd);
}



void Client::cwd(std::string & strres){
    std::cout << "cwd gotten" << std::endl;
    char msg[] = "250 dir changed\r\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    std::string inpPath(strres.begin()+ 4, strres.end());
    boost::trim_left(inpPath);
    boost::trim_right(inpPath);
    try{
       fs.cd(inpPath);
    } catch(std::exception& e){
        std::cout << e.what();
        char msg[] = "501\r\n";
        std::vector<char> cmd(msg,msg+sizeof(msg));
        (*sock).msend(cmd);
        return;
    }
    (*sock).msend(cmd);
}


void Client::mkd(std::string & strres){
    std::cout << "mkd gotten" << std::endl;
    char msg[] = "257 dir created\r\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    std::string inpPath(strres.begin()+ 4, strres.end());
    boost::trim_left(inpPath);
    boost::trim_right(inpPath);
    try{
       fs.mkdir(inpPath);
    } catch(std::exception& e){
        char msg[] = "501\r\n";
        std::vector<char> cmd(msg,msg+sizeof(msg));
        (*sock).msend(cmd);
        return;
    }
    (*sock).msend(cmd);
}

void Client::rmd(std::string & strres){
    std::cout << "rmd gotten" << std::endl;
    std::string inpPath(strres.begin()+ 4, strres.end());
    boost::trim_left(inpPath);
    boost::trim_right(inpPath);
    unsigned int res = 0;
    try{
       res = fs.rmdir(inpPath);
    } catch(std::exception& e){
        char msg[] = "501\r\n";
        std::vector<char> cmd(msg,msg+sizeof(msg));
        (*sock).msend(cmd);
        return;
    }
    std::string rstr = boost::str(boost::format("%u files deleted\r\n")%res);
    std::vector<char> cmd(rstr.begin(), rstr.end());
    (*sock).msend(cmd);
}


std::string Client::sendfile(std::shared_ptr<std::fstream>& results){
    std::string msg = "150 BINARY\r\n";
    switch (st) {
    case ASCII:
        fut = std::future<int>( std::async(std::launch::async,tcpConSFileA,datasock,results));
        msg = "150 ASCII\r\n";
        break;
    case BINARY:
        fut = std::future<int>( std::async(std::launch::async,tcpConSFileB,datasock,results));
        break;
    default:
        msg = "500 ERROR\r\n";
        break;
    }
    return msg;
}

std::string Client::storefile(std::shared_ptr<std::fstream>& results){
    std::string msg = "150 BINARY\r\n";
    switch (st) {
    case ASCII:
        fut = std::future<int>( std::async(std::launch::async,tcpConGFileA,datasock,results));
        msg = "150 ASCII\r\n";
        break;
    case BINARY:
        fut = std::future<int>( std::async(std::launch::async,tcpConGFileB,datasock,results));
        break;
    default:
        msg = "500 ERROR\r\n";
        break;
    }
    return msg;
}

void Client::retr(std::string & strres){
    std::cout << "retr gotten" << std::endl;
    std::string inpPath(strres.begin()+ 4, strres.end());
    boost::trim_left(inpPath);
    boost::trim_right(inpPath);
    std::shared_ptr<std::fstream> results;
    try{
        results = fs.file(inpPath);
    } catch(std::exception& e){
        std::cout << e.what();
        char msg[] = "501\r\n";
        std::vector<char> cmd(msg,msg+sizeof(msg));
        (*sock).msend(cmd);
        return;
    }
    std::string msg = sendfile(results);
    std::vector<char> cmd(msg.begin(),msg.end());
    (*sock).msend(cmd);
}




void Client::list(std::string & strres){
    std::cout << "list gotten" << std::endl;
    char msg[] = "150\r\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    std::string inpPath(strres.begin()+ 4, strres.end());
    boost::trim_left(inpPath);
    boost::trim_right(inpPath);
    std::string resultls;
    try{
        resultls = fs.ls(inpPath);
    } catch(std::exception& e){
        char msg[] = "501\r\n";
        std::vector<char> cmd(msg,msg+sizeof(msg));
        (*sock).msend(cmd);
        return;
    }
    std::cout<<resultls<<std::endl;
    std::cout <<"inputed path is"<<inpPath<<std::endl;
    std::vector<char> cmd2(resultls.begin(),resultls.end());
    fut = std::future<int>(std::async(std::launch::async,tcpConS,datasock,cmd2));
    (*sock).msend(cmd);

}

void Client::port(std::string & strres){
    std::regex nums("[0-9]{1,3}");
    auto beg = std::regex_iterator<std::string::iterator>(strres.begin(),strres.end(),nums);
    for(auto i = beg; i != std::regex_iterator<std::string::iterator>(); i++) {
        std::cout<< i->str() <<" ";
    }
    const char* ip[4];
    auto it = beg;
    std::string ips;
    for(int i = 0; i < 4; i++){
        //ip[i] = static_cast<char>(std::stoi((*it).str()));
        //ip[i] = (*it).str().c_str();
        ips += (*it).str();
        ips += '.';
        it++;
    }
    ips.pop_back();
    //std::string ips = boost::str(boost::format("%s.%s.%s.%s")%ip[0]%ip[1]%ip[2]%ip[3]);
    int port = std::stoi((*it).str()) * 256;
    it++;
    port += std::stoi((*it).str());
    char* msg = "200 PORT command successful. Consider using PASV\r\n";
    createDatasock();
    try{
        datasock->mconnect(const_cast<char*>(ips.c_str()),port);
    }catch(std::exception &e){
        std::cout<<e.what();
        msg =    "501                                             \r\n";
    }

    std::cout << "port gotten" << std::endl;
    std::vector<char> cmd(msg,msg+51);
    (*sock).msend(cmd);
    std::cout << "port gotten" << std::endl;
}

void Client::quit(std::string & strres){
    _finish = 1;
}


void Client::user(std::string & strres){
    std::cout << "user gotten" << std::endl;
    char msg[] = "331 username accepted\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    (*sock).msend(cmd);
}

void Client::type(std::string & strres){
    std::cout << "Type" << std::endl;
    std::cout << "list gotten" << std::endl;
    std::string inpT(strres.begin()+ 4, strres.end());
    boost::trim_left(inpT);
    boost::trim_right(inpT);
    std::string msg = "501 mod error\r\n";
    if(!inpT.compare(std::string("A"))){
        st = ASCII;
        msg = "200 mod changed\r\n";
    }
    if(!inpT.compare(std::string("I"))){
        st = BINARY;
        msg = "200 mod changed\r\n";
    }
    std::vector<char> cmd(msg.begin(),msg.end());
    (*sock).msend(cmd);
}

void Client::pass(std::string & strres){
    std::cout << "pass gotten" << std::endl;
    char msg[] = "230 Login Ok\r\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    (*sock).msend(cmd);
}
void Client::syst(std::string& strres){
    std::cout << "SYST gotten" << std::endl;
    char msg[] = "215 UNIX Type: L8 \r Remote system type is UNIX. \r Using binary mode to transfer files.\r\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    (*sock).msend(cmd);
}

void Client::accept(){
    char msg[] = "220\r\n";
    Logger& logs = Logger::Instance();
    logs.Log("new client accepted");
    std::vector<char> cmd(msg,msg+sizeof(msg));
    (*sock).msend(cmd);
}

void Client::createDatasock(){
    deleteDatasock();
    Logger& logs = Logger::Instance();
    logs.Debug("datasock created \n");
    datasock = std::shared_ptr<MyServerSocket> (new MyServerSocket());
}

void Client::deleteDatasock(){
    Logger& logs = Logger::Instance();
    logs.Debug("datasock reseted \n ");
    if (datasock){
       datasock.reset();
    }
    datasock = 0;
}

bool Client::finish() {
    return _finish;
}

void Client::execCmd() {
    if((fut.valid()&&(fut.wait_for(std::chrono::seconds(0))==std::future_status::ready))){
        finish_translation();
        return;
    }
    std::vector<char> res = (*sock).mrecv();
    std::string strres(res.begin(), res.end());
    std::cout << strres;
    Logger& logs = Logger::Instance();
    logs.Log(strres);
    std::regex user("(USER )(.*)\\r\\n");
    if (std::regex_match(strres,user)) {
        this->user(strres);
        return;
    }
    std::regex pass("(PASS )(.*)\\r\\n");
    if (std::regex_match(strres,pass)) {
        this->pass(strres);
        return;
    }
    std::regex pwd("PWD\\r\\n");
    if (std::regex_match(strres,pwd)) {
        this->pwd(strres);
        return;
    }
    std::regex syst("SYST\\r\\n");
    if (std::regex_match(strres,syst)) {
        this->syst(strres);
        return;
    }
    std::regex port("PORT ([0-9]{1,3},){5}([0-9]{1,3})\\r\\n");
    if (std::regex_match(strres,port)) {
        this->port(strres);
        return;
    }
    std::regex list("(LIST)(.*)\\r\\n");
    if (std::regex_match(strres,list)) {
        this->list(strres);
        return;
    }

    std::regex cwd("(CWD)(.*)\\r\\n");
    if (std::regex_match(strres,cwd)) {
        this->cwd(strres);
        return;
    }
    std::regex mkd("(MKD)(.*)\\r\\n");
    if (std::regex_match(strres,mkd)) {
        this->mkd(strres);
        return;
    }
    std::regex rmd("(RMD)(.*)\\r\\n");
    if (std::regex_match(strres,rmd)) {
        this->rmd(strres);
        return;
    }

    std::regex retr("(RETR)(.*)\\r\\n");
    if (std::regex_match(strres,retr)) {
        this->retr(strres);
        return;
    }
    std::regex type ("(TYPE)(.*)\\r\\n");
    if (std::regex_match(strres,type)) {
        this->type(strres);
        return;
    }
    std::regex quit ("QUIT\\r\\n");
    if (std::regex_match(strres,quit)) {
        this->quit(strres);
        return;
    }

    std::regex store ("(STOR)(.*)\\r\\n");
    if (std::regex_match(strres,store)) {
        this->store(strres);
        return;
    }
    char msg[] = "500\r\n";
    std::vector<char> cmd(msg,msg+sizeof(msg));
    (*sock).msend(cmd);
}

bool Client::hasCmd() {
    return ((sock->hasData())||(fut.valid()&&(fut.wait_for(std::chrono::seconds(0))==std::future_status::ready)));
}

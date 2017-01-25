#include "parser_arg.h"

parser_arg::commands parser_arg::get_command(std::__cxx11::string& arg_str)
{
    if(arg_str.find("-port=") == 0)
        return parser_arg::CH_PORT;
    if(arg_str.find("-dir=") == 0)
        return parser_arg::CH_WDIR;
    return parser_arg::CH_ERROR;
}

void parser_arg::_get_port_(std::__cxx11::string &arg_str)
{
    std::string port = arg_str.substr(6);
    this->port = atoi(port.data());
}

void parser_arg::_get_workdir_(std::__cxx11::string &arg_str)
{
    std::string dir = arg_str.substr(5);
    this->work_dir = dir;
}

parser_arg::parser_arg(int argc, char *argv[]):
#if _WIN32
    port(12345),work_dir(".")
#elif __linux__
    port(12345),work_dir(".")
#endif
{
    if(argc == 1)
        return;
    for(int i = 1; i<argc; ++i)
    {
        std::string buf(argv[i]);
        switch (this->get_command(buf)) {
        case parser_arg::CH_PORT:
            this->_get_port_(buf);
            break;
        case parser_arg::CH_WDIR:
            this->_get_workdir_(buf);
            break;
        default:
            throw  std::runtime_error("Argumen " +buf+" is incorrect");
            break;
        }
    }
}

std::__cxx11::string parser_arg::get_work_dir()
{
    return this->work_dir;
}

int parser_arg::get_port()
{
    return this->port;
}

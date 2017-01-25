#ifndef PARSER_ARG_H
#define PARSER_ARG_H
#include <stddef.h>
#include <string>
#include <stdexcept>
class parser_arg
{
    typedef enum {CH_PORT,CH_WDIR,CH_ERROR} commands;
    int port;
    std::string work_dir;
    commands get_command (std::__cxx11::string &arg_str);
    void _get_port_(std::__cxx11::string &arg_str);
    void _get_workdir_(std::__cxx11::string &arg_str);

public: 
    parser_arg(int argc, char *argv[]);
    std::string get_work_dir();
    int get_port();
};

#endif // PARSER_ARG_H

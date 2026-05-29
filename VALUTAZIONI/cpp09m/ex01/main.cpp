#include "RPN.hpp"
#include <iostream>


int main(int argc, char **argv)
{
    if(argc == 1)
    {
        std::cout << "USAGE: " << argv[0] << " RPN_EXPRESSION\n";
        return 1;
    }
    std::string arg = "";
    for(int i = 1; i < argc; i++)
    {
        arg += argv[i];
    }
    std::cout << "expr = "<<arg<<std::endl;
    std::cout << calc(arg.c_str()) << std::endl;
}
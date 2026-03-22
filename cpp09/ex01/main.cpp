#include "RPN.hpp"

int main (int c, char **v) {
    if(c != 2){
        std::cerr << "Only one RNP required as argument" << std::endl;
        return 1;
    }
    RPN container;
    if(container.check_error(v[1])){
        std::cerr << "Error" << std::endl;
        return 1;
    }
    container.create_stack(v[1]);
   
}
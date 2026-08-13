#include "Harl.hpp"

int main(int c, char**v) {
    if(c != 2)
        return((std::cerr << "Only one argument needed" << std::endl, 1));
    std::string levels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};
    int i = -1;
    while(++i < 4)
        if(v[1] == levels[i])
            break;
    Harl harl;
    switch(i)
    {
        case 0:
            harl.debug();
        case 1:
            harl.info();
        case 2:
            harl.warning();
        case 3:
            harl.error();
            break;
        default:
            std::cout << "[ Probably complaining about insignificant problems ]" << std::endl;
    }
}
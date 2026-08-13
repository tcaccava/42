#include "Harl.hpp"

int main(int c,char **v) {
    Harl harl;
    if (c != 2)
        return(std::cerr << "Only one argument needed" << std::endl, 1);
    std::string levels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};
    int found = 0;
    for (int i = 0; i < 4; i++){
        if (v[1] == levels[i]){
            found = 1;
            break;
        }
    }
    if(!found)
        return(std::cerr << "Harl knows only 4 words: DEBUG,INFO,WARNING,ERROR" << std::endl,1);
    harl.complain(v[1]);
    return 0;
}
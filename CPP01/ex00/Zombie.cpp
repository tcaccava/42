#include "Zombie.hpp"

Zombie::Zombie(std::string namez){
    name = namez;
}

void Zombie::announce ( void ) {
    std::cout << name << ":  " << "BraiiiiiiinnnzzzZ..." << std::endl;
}

Zombie::~Zombie(){
    std::cout << name << ": i'm a dead man" << std::endl;
}
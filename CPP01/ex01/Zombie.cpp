#include "Zombie.hpp"

Zombie::Zombie() {}

void Zombie::announce( void ) {
    std::cout << name << ":  BraiiiiiiinnnzzzZ..." << std::endl;
}

Zombie::~Zombie() {
    std::cout << name << ":  i'm gonna die..." << std::endl;
}

void Zombie::setName(std::string namez) {
    name  = namez;
}
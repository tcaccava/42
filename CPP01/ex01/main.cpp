#include "Zombie.hpp"

int main(){
    Zombie *horde = zombieHorde( 8, "Hord of zombies");
    delete[] horde;
    return 0;
}
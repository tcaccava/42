#include "HumanB.hpp"

HumanB::HumanB(std::string namez) : name(namez), weapon(nullptr) {}

void HumanB::setWeapon(Weapon& weaponz) {
    weapon = &weaponz;
}

void HumanB::attack() {
    if (weapon)
        std::cout << name << " attacks with their " << weapon->getType() << std::endl;
    else
        std::cout << name << " has no weapon to attack with" << std::endl;
}


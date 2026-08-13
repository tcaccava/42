#include "HumanA.hpp"

HumanA::HumanA(std::string namez, Weapon& weaponz): name(namez), weapon(weaponz) {}

void HumanA::attack() {
    std::cout << name << " attacks with their " << weapon.getType() << std::endl;
}



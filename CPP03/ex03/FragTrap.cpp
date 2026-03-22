#include "FragTrap.hpp"

FragTrap::FragTrap(std::string name) : ClapTrap(name)
{
    hit_points = 100;
    energy_points = 100;
    attack_damage = 30;
    std::cout << "FragTrap " << name << " constructed." << std::endl;
}

FragTrap::FragTrap(const FragTrap &other) : ClapTrap(other)
{
    std::cout << "FragTrap copy constructor called." << std::endl;
}

FragTrap &FragTrap::operator=(const FragTrap &other)
{
    if (this != &other)
        ClapTrap::operator=(other);
    std::cout << "FragTrap " << name << " copied." << std::endl;
    return (*this);
}

FragTrap::~FragTrap()
{
    std::cout << "FragTrap " << name << " destructed." << std::endl;
}

// void FragTrap::attack(const std::string &target)
// {
//     if (hit_points <= 0)
//         std::cout << "FragTrap " << name << " can't attack: no hit points left." << std::endl;
//     else if (energy_points <= 0)
//         std::cout << "FragTrap " << name << " can't attack: no energy points left." << std::endl;
//     else
//     {
//         energy_points--;
//         std::cout << "FragTrap " << name << " attacks " << target << " causing " << attack_damage << " points of damage!" << std::endl;
//     }
// }

// void FragTrap::takeDamage(unsigned int amount)
// {

//     if (hit_points <= 0)
//     {
//         std::cout << "FragTrap " << name << " is already dead!" << std::endl;
//         return;
//     }

//     hit_points = hit_points - amount;
//     if (hit_points < 0)
//         hit_points = 0;

//     std::cout << "FragTrap " << name << " suffers " << amount << " points of damage!" << std::endl;

//     if (hit_points <= 0)
//         std::cout << "FragTrap " << name << " has died!" << std::endl;
// }

// void FragTrap::beRepaired(unsigned int amount)
// {
//     if (hit_points <= 0)
//         std::cout << "FragTrap " << name << " can't repair: no hit points left." << std::endl;
//     else if (energy_points <= 0)
//         std::cout << "FragTrap " << name << " can't repair: no energy points left." << std::endl;
//     else
//     {
//         std::cout << "FragTrap " << name << " restores " << amount << " hit points!" << std::endl;
//         energy_points--;
//         hit_points = hit_points + amount;
//     }
// }

void FragTrap::highFivesGuy(void) {
    std::cout << "FragTrap " << name << " requests high fives! ✋" << std::endl;
}
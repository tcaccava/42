#include "ScavTrap.hpp"

ScavTrap::ScavTrap(const std::string name) : ClapTrap(name) {
    hit_points = 100; 
    energy_points = 50; 
    attack_damage = 20;

    std::cout << "ScavTrap " << name << " constructed." << std::endl;
}

ScavTrap::~ScavTrap()
{
    std::cout << "ScavTrap " << name << " destructed." << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap &other) : ClapTrap(other)
{
    std::cout << "ScavTrap copy constructor called." << std::endl;
}

ScavTrap &ScavTrap::operator=(const ScavTrap &other)
{
    std::cout << "ScavTrap copy assignment operator called." << std::endl;
    
     if (this != &other){
        ClapTrap::operator=(other);
    //     this->name = other.name;
    //     this->attack_damage = other.attack_damage;
    //     this->energy_points = other.energy_points;
    //     this->hit_points = other.hit_points;
     }
    std::cout << "ScavTrap " << name << " copied." << std::endl;
    return (*this);
}

void ScavTrap::attack(const std::string &target)

{
    if (hit_points <= 0)
        std::cout << "ScavTrap " << name << " can't attack: no hit points left." << std::endl;
    else if (energy_points <= 0)
        std::cout << "ScavTrap " << name << " can't attack: no energy points left." << std::endl;
    else
    {
        energy_points--;
        std::cout << "ScavTrap " << name << " attacks " << target << " causing " << attack_damage << " points of damage!" << std::endl;
    }
}
//INHERITED FUNCTIONS,NO NEED TO OVERRIDE
// void ScavTrap::takeDamage(unsigned int amount)
// {

//     if (hit_points <= 0)
//     {
//         std::cout << "ScavTrap " << name << " is already dead!" << std::endl;
//         return;
//     }

//     hit_points = hit_points - amount;
//     if (hit_points < 0)
//         hit_points = 0;

//     std::cout << "ScavTrap " << name << " suffers " << amount << " points of damage!" << std::endl;

//     if (hit_points <= 0)
//         std::cout << "ScavTrap " << name << " has died!" << std::endl;
// }
// void ScavTrap::beRepaired(unsigned int amount)
// {
//     if (hit_points <= 0)
//         std::cout << "ScavTrap " << name << " can't repair: no hit points left." << std::endl;
//     else if (energy_points <= 0)
//         std::cout << "ScavTrap " << name << " can't repair: no energy points left." << std::endl;
//     else
//     {
//         std::cout << "ScavTrap " << name << " restores " << amount << " hit points!" << std::endl;
//         energy_points--;
//         hit_points = hit_points + amount;
//     }
// }
void ScavTrap::guardGate() {
    std::cout << "ScavTrap " << name << " is now in Gate Keeper mode" << std::endl;
}
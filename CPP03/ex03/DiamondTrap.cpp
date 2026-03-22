#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap(std::string const name) : ClapTrap(name + "_clap_name"), FragTrap(name), ScavTrap(name), name(name)
{
    this->hit_points = 100;
    this->energy_points = 50;
    this->attack_damage = 30;
    std::cout << "DiamondTrap " << name << " constructed." << std::endl;
}

DiamondTrap::DiamondTrap(const DiamondTrap &other): ClapTrap(other), FragTrap(other), ScavTrap(other),name(other.name){
    std::cout << "DiamondTrap copy constructor called." << std::endl;
    }
       
DiamondTrap &DiamondTrap::operator=(const DiamondTrap &other){
    std::cout << "DiamondTrap copy assignment operator called" << std::endl;
    if (this != &other)
    {
        ClapTrap::operator=(other);
        this->name = other.name;
    }
    return (*this);
}

DiamondTrap::~DiamondTrap()
{
    std::cout << "DiamondTrap destructor called for " << name << std::endl;
}


// void DiamondTrap::attack(const std::string &target)
// {
//     ScavTrap::attack(target);
// }

void DiamondTrap::whoAmI() {
    std::cout << "I am DiamondTrap " << this->name << " and my ClapTrap name is : " << ClapTrap::name << std::endl;
}

// void DiamondTrap::takeDamage(unsigned int amount) {
//     std::cout << "[DiamondTrap custom takeDamage] ";
//     FragTrap::takeDamage(amount);
// }

// void DiamondTrap::beRepaired(unsigned int amount) {
//     std::cout << "[DiamondTrap custom beRepaired] ";
//     ScavTrap::beRepaired(amount);
// }

#include "ClapTrap.hpp"

ClapTrap::ClapTrap(std::string name) : name(name), hit_points(10),
                                       energy_points(10), attack_damage(0)
{
    std::cout << "ClapTrap " << name << " constructed." << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap &other)
{
    std::cout << "Copy constructor called." << std::endl;
    this->name = other.name;
    this->attack_damage = other.attack_damage;
    this->energy_points = other.energy_points;
    this->hit_points = other.hit_points;
}

ClapTrap &ClapTrap::operator=(const ClapTrap &other)
{
    std::cout << "Copy assignment operator called." << std::endl;
    if (this != &other)
    {
        this->name = other.name;
        this->attack_damage = other.attack_damage;
        this->energy_points = other.energy_points;
        this->hit_points = other.hit_points;
    }
    std::cout << "ClapTrap " << name << " copied." << std::endl;
    return (*this);
}

ClapTrap::~ClapTrap()
{
    std::cout << "ClapTrap " << name << " destructed." << std::endl;
}

void ClapTrap::attack(const std::string &target)
{
    if (hit_points <= 0)
        std::cout << "ClapTrap " << name << " can't attack: no hit points left." << std::endl;
    else if (energy_points <= 0)
        std::cout << "ClapTrap " << name << " can't attack: no energy points left." << std::endl;
    else
    {
        energy_points--;
        std::cout << "ClapTrap " << name << " attacks " << target << " causing " << attack_damage << " points of damage!" << std::endl;
    }
}

void ClapTrap::takeDamage(unsigned int amount)
{
    if (hit_points <= 0)
    {
        std::cout << "ClapTrap " << name << " is already dead!" << std::endl;
        return;
    }

    hit_points = hit_points - amount;
    if (hit_points < 0)
        hit_points = 0;

    std::cout << "ClapTrap " << name << " suffers " << amount << " points of damage!" << std::endl;

    if (hit_points <= 0)
        std::cout << "ClapTrap " << name << " has died!" << std::endl;
}

void ClapTrap::beRepaired(unsigned int amount)
{
    if (hit_points <= 0)
        std::cout << "ClapTrap " << name << " can't repair: no hit points left." << std::endl;
    else if (energy_points <= 0)
        std::cout << "ClapTrap " << name << " can't repair: no energy points left." << std::endl;
    else
    {
        std::cout << "ClapTrap " << name << " restores " << amount << " hit points!" << std::endl;
        energy_points--;
        hit_points = hit_points + amount;
    }
}
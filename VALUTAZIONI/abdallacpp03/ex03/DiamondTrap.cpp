#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap():ClapTrap("default_clap_name") , ScavTrap(), FragTrap(), name("default")
{
	hit_points = 100;
	energy_points = 50;
	amount = 30;
}

DiamondTrap::DiamondTrap(std::string name) : ClapTrap(name + "_clap_name") , ScavTrap(), FragTrap(), name(name)
{
	hit_points = 100;
	energy_points = 50;
	amount = 30;
}

DiamondTrap::DiamondTrap(const DiamondTrap &copy) : ClapTrap(copy), ScavTrap(copy), FragTrap (copy), name(copy.name)
{
	std::cout << "DiamondTrap copy constructor called" << std::endl;
}

DiamondTrap::~DiamondTrap()
{
	std::cout << "DiamondTrap " << name << " destructor called" << std::endl;

}

void DiamondTrap::whoAmI()
{
	std::cout << "I am " << name << " and " << ClapTrap::name << " is my ClapTrap name" << std::endl;
}

void DiamondTrap::attack(const std::string &target)
{
	ScavTrap::attack(target);
}

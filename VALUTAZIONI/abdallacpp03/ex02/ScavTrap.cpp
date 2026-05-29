#include "ScavTrap.hpp"

ScavTrap::ScavTrap() : ClapTrap()
{
	hit_points = 100;
	energy_points = 50;
	attack_damage = 20;
	std::cout << "ScavTrap default constructor called" << std::endl;
}
ScavTrap::ScavTrap(std::string name) : ClapTrap(name)
{
	this->hit_points = 100;
	this->energy_points = 50;
	this->attack_damage = 20;
	std::cout << "ScavTrap " << name << " constructor called" << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap &copy) : ClapTrap(copy)
{
	std::cout << "ScavTrap copy constructor called" << std::endl;
}

ScavTrap:: ~ScavTrap()
{
	std::cout << "ScavTrap " << name << " destructor called" << std::endl;
}

ScavTrap& ScavTrap::operator=(const ScavTrap& other)
{
	std::cout	<< "ScavTrap assignment operator called" << std::endl;
	if (this == &other)
		return (*this);
	ClapTrap::operator=(other);
	return (*this);
}

void ScavTrap::guardGate()
{
	std::cout << "ScavTrap " << name << " is now guarding the gate!" << std::endl;
}

void ScavTrap::attack(const std::string &target)
{
	if (hit_points <= 0)
	{
		std::cout << "ScavTrap " << name << " can't attack, because he is destroyed!" << std::endl;
		return ;
	}
	else if (energy_points < 1)
	{
		std::cout << "ScavTrap " << name << " can't attack, because his energy is empty!" << std::endl;
		return ;
	}
	else
	{
		std::cout << "ScavTrap " << name << " attacks " << target << " ,causing " << attack_damage << " points of damage!" << std::endl;
		energy_points--;
	}
}

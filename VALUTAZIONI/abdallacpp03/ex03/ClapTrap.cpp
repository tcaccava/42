#include "ClapTrap.hpp"

ClapTrap::ClapTrap():name("default"),hit_points(100), energy_points(50), amount(20)
{
	std::cout << "ClapTrap Default Constructor called" << std::endl;
}

ClapTrap::ClapTrap(std::string name):hit_points(10), energy_points(10), amount(0)
{
	this->name = name;
	std::cout << "ClapTrap " << name << " constructor called" << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap &copy)
{
	*this = copy;
	std::cout << "ClapTrap copy constructor called" << std::endl;
}
ClapTrap::~ClapTrap()
{
	std::cout << "ClapTrap " << name << " destructor called" << std::endl;
}

void ClapTrap::attack(const std::string& target)
{
	if (hit_points <= 0)
	{
		std::cout	<< "ClapTrap " << name << " can't attack"
					<< ", because he is destroyed!" << std::endl;
		return;
	}
	else if (energy_points < 1)
	{
		std::cout	<< "ClapTrap " << name << " can't attack"
					<< ", because his energy is empty!"
					<< std::endl;
		return;
	}
	else
	{
		std::cout	<< "ClapTrap " << name << " attacks "
					<< target << ", causing " << amount
					<< " points of damage!" << std::endl;
		energy_points--;
	}
}

void ClapTrap::takeDamage(unsigned int amount)
{
	hit_points -= amount;
	if (hit_points <= 0)
	{
		std::cout	<< "ClapTrap " << name
					<< " is destroyed!" << std::endl;
		return;
	}
	else
	{
		std::cout	<< "ClapTrap " << name << " take "
					<< amount << " points of damage!"
					<< std::endl;
	}
}

void ClapTrap::beRepaired(unsigned int amount)
{
	if (hit_points < 1)
	{
		std::cout	<< "ClapTrap " << name
					<< " can't be repaired because he is destroyed!"
					<< std::endl;
	}
	else if (energy_points <= 0)
	{
		std::cout	<< "ClapTrap " << name
					<< " can't be repaired because he has no hit points!"
					<< std::endl;
	}
	else
	{
		energy_points--;
		hit_points += amount;
		if (hit_points >= 10)
			hit_points = 10; 
		std::cout	<< "ClapTrap " << name
					<< " repair himself!\n"
					<< "ClapTrap " << name
					<< " , has now " << hit_points
					<< " hit points" << std::endl;
	}
}

ClapTrap &ClapTrap::operator=(const ClapTrap &other)
{
	std::cout	<< "ClapTrap assignment operator called" << std::endl;
	if (this == &other)
		return (*this);
	this->name = other.name;
	this->amount = other.amount;
	this->hit_points = other.hit_points;
	this->energy_points = other.energy_points;
	return (*this);

}

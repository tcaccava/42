#include "HumanA.hpp"

HumanA::HumanA(const std::string& name, Weapon& weapon )
	: name(name), weapon(weapon) {}

HumanA::~HumanA( void ) {}

void	HumanA::attack( void )
{
	std::cout	<< this->name
				<< " attacks with their "
				<< this->weapon.getType()
				<< std::endl;
}

void	HumanA::setWeapon( Weapon& weapon )
{
	this->weapon = weapon;
}

Weapon	HumanA::getWeapon( void )
{
	return (this->weapon);
}

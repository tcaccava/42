#include "HumanB.hpp"

HumanB::HumanB( const std::string& name )
	: name(name), weapon(NULL) {}

HumanB::HumanB( const std::string& name, Weapon* weapon )
	: name(name), weapon(weapon) {}

HumanB::~HumanB( void ) {}

void	HumanB::attack( void )
{
	std::cout	<< this->name
				<< " attacks with their "
				<< this->weapon->getType()
				<< std::endl;
}

void	HumanB::setWeapon( Weapon& weapon )
{
	this->weapon = &weapon;
}

Weapon	HumanB::getWeapon( void )
{
	return *(this->weapon);
}

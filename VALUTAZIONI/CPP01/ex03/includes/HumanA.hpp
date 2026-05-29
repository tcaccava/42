#ifndef HUMANA_HPP
#define HUMANA_HPP

#include "Weapon.hpp"

class HumanA
{
	private:
		std::string name;
		Weapon& weapon;
	public:
		HumanA( const std::string& name, Weapon& weapon );
		~HumanA( void );

		void	attack( void );
		void	setWeapon( Weapon& weapon );
		Weapon	getWeapon( void );

};

#endif

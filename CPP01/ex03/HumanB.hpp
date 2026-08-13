#ifndef HUMANB_HPP
# define HUMANB_HPP

# include "Weapon.hpp"
# include <iostream>
# include <string>

class HumanB
{
  public:
	HumanB(std::string namez);
	void setWeapon(Weapon &weaponz);
	void attack();

  private:
	std::string name;
	Weapon *weapon;
};

#endif
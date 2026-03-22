#ifndef HUMANA_HPP
# define HUMANA_HPP

# include "Weapon.hpp"
# include <iostream>
# include <string>

class HumanA
{
  public:
	HumanA(std::string namez, Weapon &weaponz);
	void attack();

  private:
	Weapon &weapon;
	std::string name;
};

#endif
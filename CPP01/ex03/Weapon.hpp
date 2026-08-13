#ifndef WEAPON_HPP
# define WEAPON_HPP

# include <iostream>
# include <string>

class Weapon
{
  public:
	Weapon(std::string weapon);
	const std::string& getType() const;
	void setType(const std::string& value);

  private:
	std::string type;
};

#endif
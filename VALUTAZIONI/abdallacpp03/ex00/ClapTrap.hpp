# ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

# include "iostream"

class ClapTrap
{
	private:
	std::string		name;
	int				hit_points;
	int				energy_points;
	int				attack_damage;

	public:
	ClapTrap();
	ClapTrap(std::string name);
	ClapTrap(const ClapTrap &copy);
	ClapTrap &operator=(const ClapTrap &other);
	virtual ~ClapTrap();
	void			attack(const std::string& target);
	void			takeDamage(unsigned int attack_damage);
	void			beRepaired(unsigned int attack_damage);
};

#endif

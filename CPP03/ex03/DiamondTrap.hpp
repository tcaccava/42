#ifndef DIAMONDTRAP_HPP
#define DIAMONDTRAP_HPP

#include "FragTrap.hpp"
#include "ScavTrap.hpp"

class DiamondTrap : public FragTrap, public ScavTrap {
    public:
        DiamondTrap(std::string const name);
        ~DiamondTrap();
        DiamondTrap(const DiamondTrap &other);
        DiamondTrap &operator=(const DiamondTrap& other);

        // void attack(const std::string &target);
        // void takeDamage(unsigned int amount);
        // void beRepaired(unsigned int amount);

        void whoAmI();
    private:
        std::string name;
};

#endif
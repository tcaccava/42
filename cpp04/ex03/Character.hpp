#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <iostream>
#include "ICharacter.hpp"

class Character : public ICharacter
{
public:
    Character(const std::string name);
    ~Character();
    Character(const Character &other);
    Character &operator=(const Character &other);

    void equip(AMateria *m);
    void unequip(int idx);
    void use(int idx, ICharacter &target);
    std::string const &getName() const;

private:
    AMateria *inventory[4];
    std::string name;
    AMateria *dropped[100];
};

#endif
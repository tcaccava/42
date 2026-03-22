#include "Character.hpp"
#include "AMateria.hpp"
#include "ICharacter.hpp"

Character::Character(const std::string name) : name(name)
{
    for (int i = 0; i < 4; i++)
        inventory[i] = NULL;
    for (int i = 0; i < 100; i++)
        dropped[i] = NULL;
}

Character::~Character()
{
    for (int i = 0; i < 4; i++)
    {
        if (inventory[i])
            delete inventory[i];
    }
    for (int i = 0; i < 100; i++)
        if (dropped[i])
            delete dropped[i];
}

Character::Character(const Character &other)
{
    name = other.name;
    for (int i = 0; i < 4; ++i)
        inventory[i] = NULL;
    for (int i = 0; i < 4; ++i)
        if (other.inventory[i])
            inventory[i] = other.inventory[i]->clone();
    for (int i = 0; i < 100; i++)
    {
        if (other.dropped[i])
            dropped[i] = other.dropped[i]->clone();
        else
            dropped[i] = NULL;
    }
}

Character &Character::operator=(const Character &other)
{
    if (this != &other)
    {
        for (int i = 0; i < 4; i++)
        {
            delete this->inventory[i];
            if (other.inventory[i])
                this->inventory[i] = other.inventory[i]->clone();
            else
                this->inventory[i] = NULL;
        }
    }
    this->name = other.name;
    for (int i = 0; i < 100; i++)
    {
        delete this->dropped[i];
        if (other.dropped[i])
            this->dropped[i] = other.dropped[i]->clone();
        else
            this->dropped[i] = NULL;
    }
        return *this;
}

void Character::equip(AMateria *m)
{
    int i = 0;
    while (i < 4)
    {
        if (!inventory[i])
        {
            inventory[i] = m;
            break;
        }
        else
            i++;
    }
}

void Character::unequip(int idx)
{
    if (idx < 0 || idx >= 4 || !inventory[idx])
        return;
    for (int i = 0; i < 100; i++)
    {
        if (!dropped[i])
        {
            dropped[i] = inventory[idx];
            inventory[idx] = NULL;
            return;
        }
    }
}

void Character::use(int idx, ICharacter &target)
{
    if (idx >= 0 && idx < 4 && inventory[idx])
        inventory[idx]->use(target);
}

std::string const &Character::getName() const
{
    return (name);
}

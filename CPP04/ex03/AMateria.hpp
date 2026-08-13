#ifndef AMATERIA_HPP
#define AMATERIA_HPP

#include <iostream>
#include <string>

class ICharacter;

class AMateria
{

public:
    AMateria(std::string const &type);
    virtual ~AMateria();
    AMateria(const AMateria &other);
    AMateria &operator=(const AMateria &other);

    std::string const &getType() const;
    virtual AMateria *clone() const = 0;
    virtual void use(ICharacter &target);

protected:
    std::string type;
};

#endif
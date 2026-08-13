#include "Cure.hpp"
#include "ICharacter.hpp"
#include "AMateria.hpp"

Cure::Cure() : AMateria("cure") {}

Cure::~Cure() {}

Cure::Cure(const Cure &other) : AMateria(other) {}

Cure &Cure::operator=(const Cure &other)
{
    if (this != &other)
        this->type = other.type;
    return *this;
}

void Cure::use(ICharacter &target)
{
    std::cout << "* heals " << target.getName() << "’s wounds *" << std::endl;
}

AMateria* Cure::clone() const
{
    Cure *clone = new Cure(*this);
    return clone;
}
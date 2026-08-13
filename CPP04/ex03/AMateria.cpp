#include "AMateria.hpp"

AMateria::AMateria(std::string const &type)
{
    this->type = type;
}

AMateria::~AMateria() {}

AMateria::AMateria(const AMateria &other)
{
    this->type = other.type;
}

AMateria &AMateria::operator=(const AMateria &other)
{
    if (this != &other)
        this->type = other.type;
    return *this;
}

std::string const &AMateria::getType() const
{
    return (type);
}

void AMateria::use(ICharacter& target) {
    (void)target;
}
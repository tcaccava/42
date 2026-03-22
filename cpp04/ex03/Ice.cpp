#include "Ice.hpp"
#include "ICharacter.hpp"

Ice::Ice() : AMateria("ice"){}

Ice::~Ice() {}

Ice::Ice(const Ice& other) : AMateria(other) {}

Ice &Ice::operator=(const Ice& other) {
    if(this != &other)
        this->type = other.type;
    return *this;
}

void Ice::use(ICharacter& target) {
    std::cout << "* shoots an ice bolt at " << target.getName() << " *" << std::endl;
}

AMateria* Ice::clone() const {
    Ice *clone = new Ice(*this);
    return clone;
}
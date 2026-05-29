#include "AAnimal.hpp"

AAnimal::AAnimal()
{
    std::cout << "class Animal constructor called" << std::endl;
}

AAnimal::~AAnimal()
{
    std::cout << "class Animal destructor called" << std::endl;
}

AAnimal::AAnimal(const AAnimal& other)
{
    this->type = other.type;
}

AAnimal& AAnimal::operator=(const AAnimal& other)
{
    if (this != &other)
    {
        this->type = other.type;
    }
    return (*this);
}

std::string AAnimal::getType() const
{
    return (this->type);
}



void AAnimal::makeSound() const
{
    std::cout << "generic animal doesn't make sound" << std::endl;
}

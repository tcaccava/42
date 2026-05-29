#include "Animal.hpp"

Animal::Animal()
{
    std::cout << "class Animal constructor called" << std::endl;
}

Animal::~Animal()
{
    std::cout << "class Animal destructor called" << std::endl;
}

Animal::Animal(const Animal& other)
{
    this->type = other.type;
}

Animal& Animal::operator=(const Animal& other)
{
    if (this != &other)
    {
        this->type = other.type;
    }
    return (*this);
}

std::string Animal::getType() const
{
    return (this->type);
}



void Animal::makeSound() const
{
    std::cout << "generic animal doesn't make sound" << std::endl;
}

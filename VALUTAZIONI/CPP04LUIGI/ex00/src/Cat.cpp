#include "Cat.hpp"

Cat::Cat(): Animal()
{
    std::cout << "Cat class constructor called" << std::endl;
    this->type = "Cat";
}

Cat::~Cat()
{
    std::cout << "Cat class destructor called" << std::endl;
}

Cat::Cat(const Cat& other): Animal()
{
    this->type = other.type;
}

Cat& Cat::operator=(const Cat& other)
{
    if (this != &other)
    {
        this->type = other.type;
    }
    return (*this);
}

void Cat::makeSound() const
{
    std::cout << "meow" << std::endl;
}
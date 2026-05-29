#include "Dog.hpp"

Dog::Dog(): Animal()
{
    this->type = "Dog";
    std::cout << "dog constructor called" << std::endl;
}


Dog::~Dog()
{
    std::cout << "dog destructor called" << std::endl;
}


Dog::Dog(const Dog& other): Animal()
{
    this->type = other.type;
}


Dog& Dog::operator=(const Dog& other)
{
    if (this != &other)
        this->type = other.type;
    return *this;
}


void Dog::makeSound() const
{
    std::cout << "bau bau" << std::endl;

}
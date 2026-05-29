#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal()
{
    std::cout << "wrongAnimal constructor called" << std::endl;
}

WrongAnimal::~WrongAnimal()
{
    std::cout << "wrongAnimal distructor called" << std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal& other)
{     
        this->type = other.type; 
}

WrongAnimal& WrongAnimal::operator=(const WrongAnimal& other)
{
    if (this != &other)
    {
        this->type = other.type;
    }
    return (*this);
}

void WrongAnimal::makeSound() const
{
    std::cout << "WrongAnimal sound" << std::endl;
}


std::string WrongAnimal::getType() const
{
    return "WrongAnimal type";
}

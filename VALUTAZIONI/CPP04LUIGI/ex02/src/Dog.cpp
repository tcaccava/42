#include "Dog.hpp"

Dog::Dog(): AAnimal()
{
    this->type = "Dog";
    std::cout << "dog constructor called" << std::endl;
    this->_brain = new Brain();
}


Dog::~Dog()
{
    std::cout << "dog destructor called" << std::endl;
    delete this->_brain;
    this->_brain = NULL;
    if (!this->_brain)
    {
        std::cout << "Dog Brain sucesfully deleted" << std::endl;
    }
    else
    {
        std::cout << "check DogBrain Deletion!" << std::endl;
    }
}


Dog::Dog(const Dog& other): AAnimal()
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
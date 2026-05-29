#include "Cat.hpp"
#include "Brain.hpp"

Cat::Cat(): Animal()
{
    std::cout << "Cat class constructor called" << std::endl;
    this->type = "Cat";
    this->_brain = new Brain();
}

Cat::~Cat()
{
    std::cout << "Cat class destructor called" << std::endl;
    delete this->_brain;
    this->_brain = NULL;
    if (!this->_brain)
        std::cout << "brain deleted successfully" << std::endl; 
    else
        std::cout << "check deletion of brain!!!" << std::endl;    
}   


Cat::Cat(const Cat& other): Animal()
{
    this->type = other.type;
    this->_brain = new Brain();
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
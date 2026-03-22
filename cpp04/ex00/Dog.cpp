#include "Dog.hpp"

Dog::Dog() {
    std::cout << "Dog created" << std::endl;
    type = "Dog";
}

Dog::~Dog() {
    std::cout << "Dog destructed" << std::endl;
}

Dog::Dog(const Dog& other) {
    std::cout << "Dog copy constructor called" << std::endl;
    this->type = other.type;
}

Dog &Dog::operator=(const Dog& other) {
    std::cout << "Dog copy assignment operator called" << std::endl;
    if(this != &other)
        this->type = other.type;
    return *this;
}

void Dog::makeSound() const {
    std::cout << "Bau,Bauu,Bauuuuuuuuuuuuuuuu" << std::endl;
}

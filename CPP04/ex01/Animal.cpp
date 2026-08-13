#include "Animal.hpp"

Animal::Animal() : type("none") {
    std::cout << "Animal created" << std::endl;
}

Animal::~Animal() {
    std::cout << "Animal destructed" << std::endl;
}

Animal::Animal(const Animal& other) {
    std::cout << "Animal copy constructor called" << std::endl;
    this->type = other.type;
}

Animal &Animal::operator=(const Animal& other) {
    std::cout << "Animal copy assignment operator called" << std::endl;
    if(this != &other)
        this->type = other.type;
    return *this;
}

void Animal::makeSound() const { 
    std::cout << "I'm a generic Animal,what type of sound should i make?" << std::endl;
}

std::string Animal::getType() const {
    return(type);
}
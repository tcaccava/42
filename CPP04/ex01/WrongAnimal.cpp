#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : type("none") {
    std::cout << "WrongAnimal created" << std::endl;
}

WrongAnimal::~WrongAnimal() {
    std::cout << "WrongAnimal destructed" << std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal& other) {
    std::cout << "WrongAnimal copy constructor called" << std::endl;
    this->type = other.type;
}

WrongAnimal &WrongAnimal::operator=(const WrongAnimal& other) {
    std::cout << "WrongAnimal copy assignment operator called" << std::endl;
    if(this != &other)
        this->type = other.type;
    return *this;
}

void WrongAnimal::makeSound() const { 
    std::cout << "I'm a generic WrongAnimal,what type of sound should i make?" << std::endl;
}

std::string WrongAnimal::getType() const {
    return(type);
}
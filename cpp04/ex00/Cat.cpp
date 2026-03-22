#include "Cat.hpp"

Cat::Cat() {
    std::cout << "Cat created" << std::endl;
    type = "Cat";
}

Cat::~Cat() {
    std::cout << "Cat destructed" << std::endl;
}

Cat::Cat(const Cat& other) {
    std::cout << "Cat copy constructor called" << std::endl;
    this->type = other.type;
}

Cat &Cat::operator=(const Cat& other) {
    std::cout << "Cat copy assignment operator called" << std::endl;
    if(this != &other)
        this->type = other.type;
    return *this;
}

void Cat::makeSound() const {
    std::cout << "Miao,Miao,Miaooooooooooooooo" << std::endl;
}

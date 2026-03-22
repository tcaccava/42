#include "WrongCat.hpp"

WrongCat::WrongCat() {
    std::cout << "WrongCat created" << std::endl;
    type = "WrongCat";
}

WrongCat::~WrongCat() {
    std::cout << "WrongCat destructed" << std::endl;
}

WrongCat::WrongCat(const WrongCat& other) {
    std::cout << "WrongCat copy constructor called" << std::endl;
    this->type = other.type;
}

WrongCat &WrongCat::operator=(const WrongCat& other) {
    std::cout << "WrongCat copy assignment operator called" << std::endl;
    if(this != &other)
        this->type = other.type;
    return *this;
}

void WrongCat::makeSound() const {
    std::cout << "Miao,Miao,Miaooooooooooooooo" << std::endl;
}

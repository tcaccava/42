#include "Dog.hpp"

Dog::Dog() {
    std::cout << "Dog created" << std::endl;
    type = "Dog";
    brain = new Brain();
}

Dog::~Dog() {
    std::cout << "Dog destructed" << std::endl;
    delete this->brain;
}

Dog::Dog(const Dog& other) {
    std::cout << "Dog copy constructor called" << std::endl;
    this->brain = new Brain(*other.brain);
    this->type = other.type;
}

Dog &Dog::operator=(const Dog& other) {
    std::cout << "Dog copy assignment operator called" << std::endl;
    if(this != &other){
        delete brain;
        this->brain = new Brain(*other.brain);
        this->type = other.type;
    }
    return *this;
}

void Dog::makeSound() const {
    std::cout << "Bau,Bauu,Bauuuuuuuuuuuuuuuu" << std::endl;
}

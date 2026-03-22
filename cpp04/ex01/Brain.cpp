#include "Brain.hpp"

Brain::Brain () {
    std::cout << "Brain created" << std::endl;
}

Brain::~Brain() {
    std::cout << "Brain destructed" << std::endl;
}

Brain::Brain(const Brain& other) {
    std::cout << "Brain copy constructor called" << std::endl;
    *this = other;
}

Brain &Brain::operator=(const Brain& other) {
    if(this != &other)
        for(int i = 0; i < 100; i++){
            this->ideas[i] = other.ideas[i];
        }
    return *this;
}
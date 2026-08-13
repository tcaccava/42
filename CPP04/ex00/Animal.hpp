#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class Animal {
    public:
        Animal();
        virtual ~Animal();
        Animal(const Animal& other);
        Animal &operator=(const Animal &other);
    
        virtual void makeSound() const;
        std::string getType() const;
    protected:
        std::string type;
};

#endif
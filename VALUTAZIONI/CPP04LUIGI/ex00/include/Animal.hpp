#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>

class Animal
{
   protected:
    std::string type;

   public:
    Animal();
    virtual ~Animal();
    Animal(const Animal& other);
    const Animal& operator=(const Animal& other);

    std::string getType() const;
    virtual void makeSound() const;
};

#endif
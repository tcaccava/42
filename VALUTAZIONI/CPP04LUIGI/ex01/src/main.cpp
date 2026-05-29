#include <iostream>
#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main ()
{
    int num;
    int i;

    num = 10;
    Animal *animalArray[num];

    i = 0;
    while (i < num/2)
    {
        animalArray[i] = new Cat();
        i++;
    }
    while (i < num)
    {
        animalArray[i] = new Dog();
        i++;
    }

    i = 0;
    while (i < num)
    {
        std::cout << "I'm deleting a " << animalArray[i]->getType() << ". This is the Animal n. " << i + 1 << std::endl;
        delete animalArray[i];
        i++;
    }
    Dog a;
    Dog tmp = a;
    return (0);
}
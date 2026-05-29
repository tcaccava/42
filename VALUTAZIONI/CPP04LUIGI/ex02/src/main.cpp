#include <iostream>
#include "AAnimal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main ()
{
    int num;
    int i;

    num = 10;
    AAnimal *animalArray[num];

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

    //AAnimal errorAnimal = new AAnimal;

    return (0);
}
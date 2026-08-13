#include "Dog.hpp"
#include "Cat.hpp"
#include "Animal.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include "Brain.hpp"

int main()
{
    Animal *animals[10];
    for (int i = 0; i <= 9; i++)
        (i <= 4) ? animals[i] = new Dog() : animals[i] = new Cat();
    for(int y = 0; y <= 9; y++)
        delete animals[y];
    return 0;
}
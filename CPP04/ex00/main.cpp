#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main()
{
const Animal stack_animal = Dog();//slicing
const Dog stack_dog;
const Animal* animal = new Animal();
const Animal* dog = new Dog();
const Animal* cat = new Cat();
const WrongAnimal* wrongcat = new WrongCat();
const WrongAnimal* wronganimal = new WrongAnimal();
std::cout << animal->getType() << " " << std::endl;
std::cout << stack_animal.getType() << " " << std::endl;
std::cout << stack_dog.getType() << " " << std::endl;
std::cout << dog->getType() << " " << std::endl;
std::cout << cat->getType() << " " << std::endl;
std::cout << wrongcat->getType() << " " << std::endl;
std::cout << wronganimal->getType() << " " << std::endl;
animal->makeSound();
stack_animal.makeSound();
dog->makeSound();
stack_dog.makeSound();
cat->makeSound();
wrongcat->makeSound();
wronganimal->makeSound();
delete animal;
delete dog;
delete cat;
delete wronganimal;
delete wrongcat;
return 0;
}
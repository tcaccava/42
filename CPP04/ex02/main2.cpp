#include "Dog.hpp"
#include "Cat.hpp"
#include "Animal.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"
#include "Brain.hpp"

int main()
{
    std::cout << "=== Basic Array Test ===" << std::endl;
    Animal **animals = new Animal*[10];
    for(int i = 0; i <= 9; i++)
        (i <= 4) ? animals[i] = new Dog() : animals[i] = new Cat();
    
    std::cout << "\n=== Testing Polymorphism ===" << std::endl;
    for (int y = 0; y <= 9; y++) {
        std::cout << "Animal " << y << " says: ";
        animals[y]->makeSound();
    }
    
    std::cout << "\n=== Deleting Animals ===" << std::endl;
    for (int y = 0; y <= 9; y++)
        delete animals[y];
    delete [] animals;

    std::cout << "\n=== Deep Copy Test ===" << std::endl;
    Dog original;
    std::cout << "Creating copy..." << std::endl;
    Dog copy = original;
    std::cout << "Original and copy are different objects: " 
              << (&original != &copy ? "YES" : "NO") << std::endl;

    std::cout << "\n=== Assignment Test ===" << std::endl;
    Cat cat1;
    Cat cat2;
    cat2 = cat1;

    std::cout << "\n=== Memory Leak Test ===" << std::endl;
    const Animal* j = new Dog();
    const Animal* i = new Cat();
    delete j;
    delete i;

    std::cout << "\n=== Wrong Animal Test (No Polymorphism) ===" << std::endl;
    const WrongAnimal* wrongAnimal = new WrongCat();
    const WrongCat* wrongCat = new WrongCat();
    
    std::cout << "WrongAnimal pointer calling makeSound(): ";
    wrongAnimal->makeSound();
    
    std::cout << "WrongCat pointer calling makeSound(): ";
    wrongCat->makeSound();
    
    delete wrongAnimal;
    delete wrongCat;
    

    std::cout << "\n=== Test Completed ===" << std::endl;
    return 0;
}
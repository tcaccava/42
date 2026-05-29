#include <iostream>
#include "../inc/Phonebook.hpp"
#include "../inc/Contact.hpp"

int main() {
    std::string command;
    PhoneBook   phoneBook;

    while (true)
    {
        std::cout << "Enter command: (ADD / SEARCH / EXIT)" << std::endl;
        std::cin >> command;
        if (command == "ADD")
            phoneBook.addContact();
        else if (command == "SEARCH")
            phoneBook.search();
        else if (command == "EXIT")
            break ;
    }
    return 0;
}

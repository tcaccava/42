# include "PhoneBook.hpp"
int main() {
    std::string input;
    PhoneBook phonebook;
    while(1) {
    std::cout << "Phonebook : please insert a command(ADD,SEARCH,EXIT): ";
    std::getline(std::cin,input);
    if(input == "ADD")
        phonebook.addContact();
    else if(input == "SEARCH")
        phonebook.searchContact();
    else if (input == "EXIT")
        break;
    else    
        std::cout << "COMMAND UNKNOWN" << std::endl;
    }
}
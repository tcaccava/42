#include "PhoneBook.hpp"
#include <iostream>
#include <string>

int main()
{
	PhoneBook phoneBook;
	std::string command;
	std::cout << "Welcome to the PhoneBook!" << std::endl;	
	while (true)
	{
		command = phoneBook.getCommand();
		if (command == "ADD")
			phoneBook.addContact();
		else if (command == "SEARCH")
			phoneBook.searchContacts();
		else if (command == "EXIT")
			break;
		else
			std::cout << "Invalid command. Please use ADD, SEARCH, or EXIT." << std::endl;
	}
	return 0;
}
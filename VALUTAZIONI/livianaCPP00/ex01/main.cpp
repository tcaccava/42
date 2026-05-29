#include "PhoneBook.hpp"

int	main()
{
	std::string command;
	PhoneBook phonebook;

	while (1)
	{
		if (!std::cin)
			break ;
		std::cout << "\033[1;81mWHAT WOULD YOU LIKE TO DO? \033[0mtype:" << std::endl;
		std::cout << "\033[1;81mADD\033[0m to save a new contact" << std::endl;
		std::cout << "\033[1;81mSEARCH\033[0m to display a specific contact" << std::endl;
		std::cout << "\033[1;81mEXIT\033[0m if you want to quit the program" << std::endl;
		std::getline(std::cin, command);
		if (command == "ADD")
			phonebook.add();
		else if (command == "EXIT")
			phonebook.ft_exit();
		else if (command == "SEARCH")
			phonebook.search();
	}
	return 0;
}

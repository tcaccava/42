#include "PhoneBook.hpp"
#include "Contact.hpp"

PhoneBook::PhoneBook(void)
{
	id = 0;
};

void	PhoneBook::add()
{
	std::string first_name;
	std::string last_name;
	std::string	nickname;
	std::string	number;
	std::string secret;

	first_name = get_input("first name:");
	last_name = get_input("last name:");
	nickname = get_input("nickname:");
	number = get_input("phone number:");
	secret = get_input("darkest secret:");
	list[id % 8] = Contact(first_name, last_name, nickname, number, secret);
	this->id += 1;
}

void	PhoneBook::ft_exit()
{
	std::cout << "\033[0;93mThank You For Using PhoneBook\033[0m" << std::endl;
	exit(0);
}

void	PhoneBook::search()
{
	Contact 	contact;
	int 		i;

	if (id == 0)
	{
		std::cout << "\033[1;91mNo contact saved\033[0m" << std::endl;
		return ;
	}
	std::cout << "\033[1;81mType the id of the contact you need to know\033[0m" << std::endl;
	for (i = 0; i < 8; i++)
	{
		if (i < this->id)
		{
			contact.show_contact(list[i], i);
			std::cout << std::endl;
		}
	}
	this->get_id();
}

void	PhoneBook::get_id()
{
	Contact		contact;
	char		number;
	std::string	input;

	number = '1';
	std::getline(std::cin, input);
	while (number != '9')
	{
		if (std::atoi(input.c_str()) <= id && input[0] == number && !input[1])
		{
			contact.print_contact(list[std::atoi(input.c_str()) - 1]);
			return ;
		}
		number++;
	}
	std::cout << "\033[1;91minvalid id\033[0m" << std::endl;
	if (!std::cin)
		return ;
	std::cout << "\033[1;81mType the id of the contact you need to know\033[0m" << std::endl;
	get_id();
};

std::string	PhoneBook::get_input(std::string to_print)
{
	std::string input;

	while (input.size() == 0)
	{
		if (!std::cin)
			break ;
		std::cout << "\033[1;81m" << to_print << "\033[0m" << std::endl;
		std::getline(std::cin, input);
	}
	return (input);
}
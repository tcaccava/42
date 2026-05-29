#include "Classes.hpp"

Contact::Contact()
{
	;
}

Contact::~Contact()
{
	;
}

bool notnumbers(std::string str)
{
	for (std::string::size_type i = 0; i < str.length(); i++)
	{
		if (str[i] < 48 || str[i] > 57)
			return (true);
	}
	return (false);
}

bool allspaces(std::string str)
{
	for (std::string::size_type i = 0; i < str.length(); i++)
	{
		if (!isspace(str[i]))
			return (false);
	}
	return (true);
}

void	Contact::add()
{
	std::cout << std::endl << "Enter first name: ";
	std::getline(std::cin, this->first_name);
	if (std::cin.eof())
		exit (0);
	while (allspaces(this->first_name))
	{
		std::cout << "Error, field can't be blank: ";
		std::getline(std::cin, this->first_name);
		if (std::cin.eof())
			exit (0);
	}

	std::cout << "Enter last name: ";
	std::getline(std::cin, this->last_name);
	if (std::cin.eof())
		exit (0);
	while (allspaces(this->last_name))
	{
		std::cout << "Error, field can't be blank: ";
		std::getline(std::cin, this->last_name);
		if (std::cin.eof())
			exit (0);
	}

	std::cout << "Enter nickname: ";
	std::getline(std::cin, this->nickname);
	if (std::cin.eof())
		exit (0);
	while (allspaces(this->nickname))
	{
		std::cout << "Error, field can't be blank: ";
		std::getline(std::cin, this->nickname);
		if (std::cin.eof())
			exit (0);
	}

	std::cout << "Enter phone number: ";
	std::getline(std::cin, this->phonenumber);
	if (std::cin.eof())
		exit (0);
	while (this->phonenumber.length() == 0 || notnumbers(this->phonenumber))
	{
		std::cout << "Error, you can enter only numbers: ";
		std::getline(std::cin, this->phonenumber);
		if (std::cin.eof())
			exit (0);
	}

	std::cout << "Enter darkest secret: ";
	std::getline(std::cin, this->darkest_secret);
	if (std::cin.eof())
		exit (0);
	while (allspaces(this->darkest_secret))
	{
		std::cout << "Error, field can't be blank: ";
		std::getline(std::cin, this->darkest_secret);
		if (std::cin.eof())
			exit (0);
	}
	std::cout << std::endl;
}

void PhoneBook::add(void)
{
	if (this->len == 8)
	{
		for (int i = 1; i < 8; i++)
			contacts[i-1] = contacts[i];
		this->len--;
		this->contacts[7].~Contact();
	}
	this->contacts[len] = Contact();
	this->contacts[len].add();
	this->len++;
}

void	Contact::print_one_line()
{
	if (this->first_name.length()>10)
		std::cout << std::right << this->first_name.substr(0, 9) + ".";
	else
		std::cout << std::setw(10) << std::right << std::setfill(' ') << this->first_name;
	std::cout << "|";
	if (this->last_name.length()>10)
		std::cout << std::right << this->last_name.substr(0, 9) + ".";
	else
		std::cout << std::setw(10) << std::right << std::setfill(' ') << this->last_name;
	std::cout << "|";
	if (this->nickname.length()>10)
		std::cout << std::right << this->nickname.substr(0, 9) + ".";
	else
		std::cout << std::setw(10) << std::right << std::setfill(' ') << this->nickname;
	std::cout << std::endl;
}

void	Contact::print_more_lines()
{
	std::cout << std::endl << "Name: " << this->first_name << std::endl << "Last name: " << this->last_name << std::endl << "Nickname: " << this->nickname << std::endl << "Number: " << this->phonenumber << std::endl << "Darkest secret: " << this->darkest_secret << std::endl << std::endl;
}

void PhoneBook::search(void)
{
	std::string	index;

	if (!this->len)
	{
		std::cout << std::endl << "PhoneBook is empty!" << std::endl << std::endl;
		return ;
	}
	std::cout << std::endl << "     index|first name|  lastname|  nickname" << std::endl;
	for (int i = 0; i < this->len; i++)
	{
		std::cout << "         " << i + 1 << "|";
		contacts[i].print_one_line();
	}
	std::cout << std::endl << "Choose index: ";
	std::getline(std::cin, index);
	if (std::cin.eof())
		exit (0);
	while (index.length()!=1 || index[0]<49 || index[0]>48 + this->len)
	{
		if (this->len == 1)
			std::cout << "Error, you have to choose 1: ";
		else
			std::cout << "Error, choose a number between 1 and " << this->len << ": ";
		std::getline(std::cin, index);
		if (std::cin.eof())
			exit (0);
	}
	contacts[(int)index[0]-49].print_more_lines();
}

PhoneBook::PhoneBook()
{
	this->len = 0;
}

PhoneBook::~PhoneBook()
{
	for (int i = 0; i < this->len; i++)
		this->contacts[i].~Contact();
}

int	main()
{
	std::string	input;
	PhoneBook	book;

	book = PhoneBook();
	do
	{
		std::cout << "Enter a command: ADD, SEARCH, EXIT" << std::endl << ": ";
		std::getline(std::cin, input);
		if (std::cin.eof())
			return (0);
		if (!input.compare("ADD"))
			book.add();
		else if (!input.compare("SEARCH"))
			book.search();
	} while (input.compare("EXIT"));
}
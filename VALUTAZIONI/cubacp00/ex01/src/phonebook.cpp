#include "../inc/Phonebook.hpp"
#include <cstdlib>
#include <stdio.h>

PhoneBook::PhoneBook() {
	this->contacts_saved = 0;
	this->oldest_index = 0;
}

void    PhoneBook::addContact() {
	std::string firstName, lastName, nickname, phoneNumber, darkestSecret;

	std::cout << "\n\n* ADDING NEW CONTACT *" << std::endl;
	std::cout << "First Name: ";
	std::getline(std::cin >> std::ws, firstName);

	std::cout << "Last Name: ";
	std::getline(std::cin >> std::ws, lastName);

	std::cout << "Nickname: ";
	std::getline(std::cin >> std::ws, nickname);

	std::cout << "Phone Number: ";
	std::getline(std::cin >> std::ws, phoneNumber);

	std::cout << "Darkest Secret: ";
	std::getline(std::cin >> std::ws, darkestSecret);

	this->contacts[this->oldest_index].update(firstName, lastName, nickname, phoneNumber, darkestSecret);
	this->oldest_index = (this->oldest_index + 1) % CONTACTS_NUM;
	if (this->contacts_saved >= CONTACTS_NUM)
		this->contacts_saved = CONTACTS_NUM;
	else
		this->contacts_saved++;
}

void PhoneBook::printHeader() const
{
	std::cout<<"---------------------------------------------"<<std::endl;
	std::cout<<"|                  CONTACTS                 |"<<std::endl;
	std::cout<<"---------------------------------------------"<<std::endl;
}


void PhoneBook::printValue(std::string value) const
{
	if (value.size() <= COLUMN_LEN)
		std::cout<<std::setw(COLUMN_LEN) << value << "|";
	else
		std::cout<< value.erase(COLUMN_LEN - 1) << ".|";
}

static bool	isDigit(char c)
{
	return (c >= '0' && c <='9');
}

void    PhoneBook::search() const
{
	Contact     temp;
	std::string indexStr;
	int			index;

	printHeader();
	for (size_t i = 0; i < this->contacts_saved; i++)
	{
		std::cout << "|";
		std::cout << std::setw(COLUMN_LEN);
		std::cout << i << "|";
		printValue(this->contacts[i].getFirstName());
		printValue(this->contacts[i].getLastName());
		printValue(this->contacts[i].getNickname());
		std::cout<<std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	index = -1;
	while (index == -1)
	{
		std::cout << "Please insert desired index to see contact details:" << std::endl;
		std::cin >> indexStr;
		if (indexStr.size() == 0)
			continue ;
		if (isDigit(indexStr[0]))
			index = std::atoi(indexStr.c_str());
		if (index < 0 || index > CONTACTS_NUM - 1 || index >= (int)contacts_saved)
		{
			std::cout << "Index invalid. Please select a valid index" << std::endl;
			index = -1;
		}
	}
	
	std::cout << "\n\nContact num. " << index << " details:" << std::endl;
	this->contacts[index].printInfo();
	std::cout << std::endl;
}


PhoneBook::PhoneBook ( const PhoneBook &contact ) {
	size_t i;

	for (i = 0; i < contact.contacts_saved; i++)
		this->contacts[i] = contact.contacts[i];

	while (i < CONTACTS_NUM) {
		this->contacts[i] = Contact();
		i++;
	}
}

PhoneBook::~PhoneBook () {

}

PhoneBook & PhoneBook::operator=(const PhoneBook &contact) {
	size_t i;

	for (i = 0; i < contact.contacts_saved; i++)
		this->contacts[i] = contact.contacts[i];

	while (i < CONTACTS_NUM) {
		this->contacts[i] = Contact();
		i++;
	}
	return *this;
}
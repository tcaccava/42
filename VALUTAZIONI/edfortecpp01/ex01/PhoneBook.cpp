#include "PhoneBook.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

PhoneBook::PhoneBook()
{
	currentIndex = 0;
	contactCount = 0;
}

std::string PhoneBook::truncateString(std::string str) const
{
	if (str.length() > 10)
		str = str.substr(0, 8) + ".";
	return str;
}

bool PhoneBook::isValidPhoneNumber(std::string phone) const
{
	if (phone.empty()) return false;
	std::stringstream ss(phone);
	long long number;
	if (ss >> number && ss.eof())
	{
		return true;
	}
	return false;
}

std::string PhoneBook::getInput(std::string prompt, std::string fieldType) const
{
	std::string input;
	while (true)
	{
		std::cout << prompt;
		std::getline(std::cin, input);
		size_t start = input.find_first_not_of(" \t");
		if (start == std::string::npos)
		{
			std::cout << "Field cannot be empty. Please try again." << std::endl;
			continue;
		}
		size_t end = input.find_last_not_of(" \t");
		input = input.substr(start, end - start + 1);
		if (input.empty())
		{
			std::cout << "Field cannot be empty. Please try again." << std::endl;
			continue;
		}
		if (fieldType == "phone" && !isValidPhoneNumber(input))
		{
			std::cout << "Phone number must contain only digits. Please try again." << std::endl;
			continue;
		}
		break;
	}
	return input;
}

bool PhoneBook::isValidIndex(int index) const
{
	return (index >= 0 && index < 8 && !contacts[index].isEmpty());
}

void PhoneBook::addContact()
{
	Contact newContact;
	std::cout << "Adding a new contact..." << std::endl;
	std::string firstName = getInput("First Name:\n");
	newContact.setFirstName(firstName);
	std::string lastName = getInput("Last Name:\n");
	newContact.setLastName(lastName);
	std::string nickname = getInput("Nickname:\n");
	newContact.setNickname(nickname);
	std::string phoneNumber = getInput("Phone Number:\n", "phone");
	newContact.setPhoneNumber(phoneNumber);
	std::string darkestSecret = getInput("Darkest Secret:\n");
	newContact.setDarkestSecret(darkestSecret);
	contacts[currentIndex] = newContact;
	currentIndex = (currentIndex + 1) % 8;
	if (contactCount < 8)
		contactCount++;
	std::cout << "Contact added successfully!\n" << std::endl;
}

void PhoneBook::displayContactList() const
{
	std::cout << std::setw(10) << "Index" << "|";
	std::cout << std::setw(10) << "First Name" << "|";
	std::cout << std::setw(10) << "Last Name" << "|";
	std::cout << std::setw(10) << "Nickname" << std::endl;
	for (int i = 0; i < 8; i++)
	{
		if (!contacts[i].isEmpty())
		{
			std::cout << std::setw(10) << i << "|";
			std::cout << std::setw(10) << truncateString(contacts[i].getFirstName()) << "|";
			std::cout << std::setw(10) << truncateString(contacts[i].getLastName()) << "|";
			std::cout << std::setw(10) << truncateString(contacts[i].getNickname()) << std::endl;
		}
	}
	std::cout << "\n";
}

void PhoneBook::displayContact(int index) const
{
	if (!isValidIndex(index))
	{
		std::cout << "Invalid index or no contact at this position!" << std::endl;
		return;
	}
	std::cout << "-----Contact details-----" << std::endl;
	contacts[index].displayFull();
	std::cout << "-------------------------" << std::endl;
	std::cout << std::endl;
}

void PhoneBook::searchContacts()
{
	if (contactCount == 0)
	{
		std::cout << "Phonebook is empty!" << std::endl;
		return;
	}	
	displayContactList();
	std::string input;
	int index;
	while (true)
	{
		std::cout << "Enter the index of the contact to display: ";
		std::getline(std::cin, input);
		std::stringstream ss(input);
		if (ss >> index && ss.eof())
		{
			displayContact(index);
			break;
		} else
			std::cout << "Invalid input. Please enter a valid index." << std::endl;
	}
}

std::string PhoneBook::getCommand()
{
	std::string command;
	std::cout << "Enter a command (ADD, SEARCH, EXIT): ";
	std::getline(std::cin, command);
	std::cout << "\n";
	return command;
}
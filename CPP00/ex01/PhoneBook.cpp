#include "./PhoneBook.hpp"

PhoneBook::PhoneBook() : index(0), total_contacts(0) {}

void PhoneBook::addContact()
{
	Contacts[index].setContact();
	index++;
	if (index > 7)
		index = 0;
	if (total_contacts < 8)
		total_contacts++;
}
static std::string formatField(const std::string field)
{
	if (field.length() > 10)
		return (field.substr(0, 9) + '.');
	else
		return (field);
}
void PhoneBook::searchContact()
{
	std::cout << std::setw(10) << "Index" << "|";
	std::cout << std::setw(10) << "First name" << "|";
	std::cout << std::setw(10) << "Last name" << "|";
	std::cout << std::setw(10) << "Nickname" << "|" << std::endl;
	for (int i = 0; i < total_contacts; i++)
		std::cout << std::setw(10) << i << "|" << std::setw(10) << formatField(Contacts[i].getFirstName()) << "|" << std::setw(10) << formatField(Contacts[i].getLastName()) << "|" << std::setw(10) << formatField(Contacts[i].getNickname()) << "|" << std::endl;
	std::string input;
	std::cout << "Enter the index to view full contact: ";
	std::getline(std::cin, input);
	if (input.length() != 1 || input[0] < 48 || input[0] > 55)
	{
		std::cout << "Invalid index" << std::endl;
		return ;
	}
	int index = input[0] - 48;
	if (index >= total_contacts)
	{
		std::cout << "No contacts for this index" << std::endl;
		return ;
	}
	Contacts[index].printDetails();
}
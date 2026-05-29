#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include "Contact.hpp"
#include <string>

class PhoneBook
{
private:
	Contact contacts[8];
	int currentIndex;
	int contactCount;

	std::string truncateString(std::string str) const;
	std::string getInput(std::string prompt, std::string fieldType = "") const;
	bool isValidPhoneNumber(std::string phone) const;
	bool isValidIndex(int index) const;

public:
	PhoneBook();
	
	void addContact();
	void searchContacts();
	void displayContactList() const;
	void displayContact(int index) const;
	
	std::string getCommand();
};

#endif
#include "Contact.hpp"

void Contact::setContact()
{
	while (first_name.empty()) {
		std::cout << "Enter first name: ";
		std::getline(std::cin, first_name);
	}
	while (last_name.empty()) {
		std::cout << "Enter last name: ";
		std::getline(std::cin, last_name);
	}
	while (nickname.empty()) {
		std::cout << "Enter nickname: ";
		std::getline(std::cin, nickname);
	}
	while (phone_number.empty()) {
		std::cout << "Enter phone number: ";
		std::getline(std::cin, phone_number);
	}
	while (darkest_secret.empty()) {
		std::cout << "Enter darkest secret: ";
		std::getline(std::cin, darkest_secret);
	}
}

void Contact::printDetails() const
{
	std::cout << "First name: " << first_name << std::endl;
	std::cout << "Last name: " << last_name << std::endl;
	std::cout << "Nickname: " << nickname << std::endl;
	std::cout << "Phone Number: " << phone_number << std::endl;
	std::cout << "Darkest Secret: " << darkest_secret << std::endl;
}

std::string Contact::getFirstName() const
{
	return (first_name);
}

std::string Contact::getLastName() const
{
	return (last_name);
}

std::string Contact::getNickname() const
{
	return (nickname);
}
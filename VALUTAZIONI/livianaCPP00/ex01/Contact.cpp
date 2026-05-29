/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:02:28 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/27 23:02:37 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"

Contact::Contact(void)
{};

Contact::Contact(std::string first_name, std::string last_name, std::string nickname, std::string number, std::string secret)
{
	this->first_name = first_name;
	this->last_name = last_name;
	this->nickname = nickname;
	this->number = number;
	this->secret = secret;
};

void	Contact::print_contact(Contact contact)
{	std::cout << "\033[1;95mFIRST NAME:\033[0m " << contact.first_name << std::endl;
	std::cout << "\033[1;95mLAST NAME:\033[0m " << contact.last_name << std::endl;
	std::cout << "\033[1;95mNICKNAME:\033[0m " << contact.nickname << std::endl;
	std::cout << "\033[1;95mPHONE NUMBER:\033[0m " << contact.number << std::endl;
	std::cout << "\033[1;95mDARKEST SECRET:\033[0m " << contact.secret << std::endl;
}

void	Contact::check_and_print_field(std::string string)
{
	int	length;
	int i;

	length = string.length();
	if (length == 10)
	{
		std::cout << string << " \033[1;91m|\033[0m ";
		return ;
	}
	if (length > 10)
	{
		for (i = 0; i < 9; i++)
			std::cout << string[i];
		std::cout << ".";
	}
	else
	{
	while (length < 10)
		{
			std::cout << " ";
			length++;
		}
		std::cout << string;
	}
	std::cout << " \033[1;91m|\033[0m ";
}

void	Contact::show_contact(Contact contact, int id)
{
	std::cout << "         " << id + 1 << " \033[1;91m|\033[0m ";
	check_and_print_field(contact.first_name);
	check_and_print_field(contact.last_name);
	check_and_print_field(contact.nickname);
}

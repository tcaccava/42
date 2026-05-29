/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:02:47 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/27 23:03:15 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <iostream>
#include <string>
#include <cstdlib>

class Contact
{
	public:
		Contact(void);
		Contact(std::string first_name, std::string last_name, std::string nickname, std::string number, std::string secret);
		void show_contact(Contact contact, int id);
		void print_contact(Contact contact);

	private:
		std::string first_name;
		std::string last_name;
		std::string nickname;
		std::string number;
		std::string secret;
		void check_and_print_field(std::string string);
};

#endif

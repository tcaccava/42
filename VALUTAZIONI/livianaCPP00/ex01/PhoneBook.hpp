/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:03:38 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/27 23:03:41 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

#include <iostream>
#include <string>
#include "Contact.hpp"
#include <cstdlib>

class PhoneBook {

	public:
		PhoneBook();
		void add(void);
		void search(void);
		void ft_exit(void);

	private:
		int	id;
		Contact		list[8];
		void		get_id(void);
		std::string	get_input(std::string);
};

#endif

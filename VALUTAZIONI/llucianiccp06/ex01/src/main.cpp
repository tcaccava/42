/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:24:03 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/27 16:24:08 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serialize.hpp"
#include "Data.hpp"


int main()
{
	Data s;
	s.str = "Hello World";

	Data *data = &s; 

	uintptr_t ptr = Serialize::serialize(data);
	Data *data2 = Serialize::deserialize(ptr);
	
	std::cout << "address of ptr is: " << data2 << std::endl;
	std::cout << "address of data is: " << data << std::endl;

	if(data2 == data)
		std::cout << "the address is the same" << std::endl;
}

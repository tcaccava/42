/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:04:00 by apintaur          #+#    #+#             */
/*   Updated: 2025/09/04 09:38:13 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serializer.hpp"
#include <iostream>
#include <cstdlib>

int	main() {

	Data		d1, d2;
	uintptr_t	u1, u2;

	d1.value = 1;
	d2.value = 4;
	u1 = u2 = 0x0;

	std::cout << "== BEFORE SERIALIZATION == " << std::endl;
	std::cout << "d1 address = " << &d1 << std::endl;
	std::cout << "d2 address = " << &d2 << std::endl;
	std::cout << "d1.value = " << d1.value << std::endl;
	std::cout << "d2.value = " << d2.value << "\n" << std::endl;

	u1 = Serializer::serialize(&d1);
	u2 = Serializer::serialize(&d2);

	std::cout << "== AFTER SERIALIZATION == " << std::endl;
	std::cout << "d1 address = " << &d1 << std::endl;
	std::cout << "d2 address = " << &d2 << std::endl;
	std::cout << "d1.value = " << d1.value << std::endl;
	std::cout << "d2.value = " << d2.value << std::endl;
	std::cout << "u1 value = " << u1 << std::endl;
	std::cout << "u2 value = " << u2 << "\n" << std::endl;

	Data	*d1_p, *d2_p;

	d1_p = d2_p = NULL;

	d1_p = Serializer::deserialize(u1);
	d2_p = Serializer::deserialize(u2);

	std::cout << "== AFTER DESERIALIZATION (using uintptr_t addresses) == " << std::endl;
	std::cout << "d1 address = " << d1_p << std::endl;
	std::cout << "d2 address = " << d2_p << std::endl;
	std::cout << "d1.value = " << d1_p->value << std::endl;
	std::cout << "d2.value = " << d2_p->value << std::endl;
	std::cout << "u1 value = " << u1 << std::endl;
	std::cout << "u2 value = " << u2 << "\n" << std::endl;

}

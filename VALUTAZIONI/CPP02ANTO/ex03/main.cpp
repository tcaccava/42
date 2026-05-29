/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:17:06 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/09 15:17:07 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Point.hpp"

std::string	getResult( const bool& result ) {
	if (result == 0) {
		return ("false");
	}
	return ("true");
}

int main() {
	Point A(0, 0);
	Point B(10, 0);
	Point C(0, 10);

	// Caso 1: punto interno
	Point P1(2, 2);
	std::cout << "P1 (2,2) inside: " << getResult(bsp(A, B, C, P1)) << std::endl;

	// Caso 2: punto su un lato (AB)
	Point P2(5, 0);
	std::cout << "P2 (5,0) on edge AB: " << getResult(bsp(A, B, C, P2)) << std::endl;

	// Caso 3: punto su un vertice (A)
	Point P3(0, 0);
	std::cout << "P3 (0,0) on vertex A: " << getResult(bsp(A, B, C, P3)) << std::endl;

	// Caso 4: punto esterno
	Point P4(10, 10);
	std::cout << "P4 (10,10) outside: " << getResult(bsp(A, B, C, P4)) << std::endl;

	// Caso 5: punto quasi sul bordo (precisione)
	Point P5(0.1f, 0.1f);
	std::cout << "P5 (0.1,0.1) near edge: " << getResult(bsp(A, B, C, P5)) << std::endl;

	return 0;
}

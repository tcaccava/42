/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:54:18 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/22 11:12:40 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Point.hpp"

int main( void ) {
	Point A(0,0);
	Point B(4,0);
	Point C(2,4);
	
	// 0 = false 1 = true
	//Point P(2,1); //in 1
	//Point P(2,2); //in 1
	//Point P(2.5f, 2);// in 1
	//Point P(2, 0.0039f); in 1

	//Point P(5,1); //out 0
	//Point P(0,4); //out 0
	//Point P(2, -1); // out 0
	//Point P(2, -0.0039f); // out 0
	
	//Point P(2,0); //edge 0
	//Point P(1,2); //edge 0
	Point P(3,2); //edge 0

	std::cout << "A: " << A << std::endl;
	std::cout << "B: " << B << std::endl;
	std::cout << "C: " << C << std::endl;
	std::cout << "P: " << P << std::endl;

	std::cout << "[Float Version] Is Inside: " << bsp_float(A, B, C, P) << std::endl;
	std::cout << "[Fixed Version] Is Inside: " << bsp_fixed(A, B, C, P) << std::endl;
	return 0;
}
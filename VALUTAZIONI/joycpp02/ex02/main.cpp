/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarnebi <jcarnebi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:53:24 by jcarnebi          #+#    #+#             */
/*   Updated: 2025/07/22 18:35:03 by jcarnebi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Fixed.hpp"

int main( void ) {
    Fixed a;
    Fixed const b( Fixed( 5.05f ) * Fixed( 2 ) );
    std::cout << a << std::endl;
    std::cout << ++a << std::endl;
    std::cout << a << std::endl;
    std::cout << a++ << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << Fixed::max( a, b ) << std::endl;
    /*
    // comparison operators tests
    Fixed a(10);
    Fixed b(20);
    std::cout << "a < b: " << (a < b) << std::endl;
    std::cout << "a > b: " << (a > b) << std::endl;
    std::cout << "a <= b: " << (a <= b) << std::endl;
    std::cout << "a >= b: " << (a >= b) << std::endl;
    std::cout << "a == b: " << (a == b) << std::endl;
    std::cout << "a != b: " << (a != b) << std::endl;
    
    // arithmetic operators tests
    Fixed a(5.5f);
    Fixed b(2);
    std::cout << "a: " << a << ", b: " << b << std::endl;
    std::cout << "a + b: " << (a + b) << std::endl;
    std::cout << "a - b: " << (a - b) << std::endl;
    std::cout << "a * b: " << (a * b) << std::endl;
    std::cout << "a / b: " << (a / b) << std::endl;

    // min and max tests
    Fixed a(10);
    Fixed b(20);
    const Fixed c(5.5f);
    const Fixed d(10.1f);

    // min e max non-const
    std::cout << "min(a, b): " << Fixed::min(a, b) << std::endl; // 10
    std::cout << "max(a, b): " << Fixed::max(a, b) << std::endl; // 20

    // min e max const
    std::cout << "min(c, d): " << Fixed::min(c, d) << std::endl; // 5.5
    std::cout << "max(c, d): " << Fixed::max(c, d) << std::endl; // 10.1016
    */
    return 0;
}
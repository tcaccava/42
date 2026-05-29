/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:10:28 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:10:28 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "whatever.hpp"


int main()
{
    float e = 5.5;
    float f = 6.8;

    swap(e , f);
    std::cout << "the min is: " << min(e, f) << std::endl;
    std::cout << "the max is: " << max(e, f) << std::endl;
    std::cout << "e: " << e << " f: " << f << std::endl;

    int a = 2;
    int b = 3;
    ::swap( a, b );
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "min( a, b ) = " << ::min( a, b ) << std::endl;
    std::cout << "max( a, b ) = " << ::max( a, b ) << std::endl;
    std::string c = "chaine1";
    std::string d = "chaine2";
    ::swap(c, d);
    std::cout << "c = " << c << ", d = " << d << std::endl;
    std::cout << "min( c, d ) = " << ::min( c, d ) << std::endl;
    std::cout << "max( c, d ) = " << ::max( c, d ) << std::endl;
}
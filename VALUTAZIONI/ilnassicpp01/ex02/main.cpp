/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 17:07:01 by ilnassi           #+#    #+#             */
/*   Updated: 2026/05/30 17:07:03 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

int main()
{
    std::string Str;
    std::string *strPTR;

    Str = "HI THIS IS BRAIN";
    strPTR = &Str;
    std::string &strREF = Str;
    
    std::cout << "The string address: " << &Str << std::endl;
    std::cout << "The adress in stringPTR : " << strPTR << std::endl;
     std::cout << "The adress of stringPTR : " << &strPTR << std::endl;
    std::cout << "The adress in stringREF : " << &strREF << std::endl;
    std::cout << "The string value: " << Str << std::endl;
    std::cout << "The value in stringPTR : " << *strPTR << std::endl;
    std::cout << "The value in stringREF : " << strREF << std::endl;
    strREF = "ciao";
    std::cout << "The value in stringREF : " << strREF << std::endl;
    std::cout << "The string value: " << Str << std::endl;

    return (0);
}

char *str = NULL;
*str = "tobia";

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:58:27 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/27 16:58:27 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"


Base* generate(void)
{
	std::srand(std::time(NULL));

	int i = rand();

	if(i % 5 == 0)
		return new A();
	else if(i % 2 == 0)
		return new B();
	else
		return new C();
}

void identify(Base* p)
{
	if(dynamic_cast<A*>(p) != NULL)
		std::cout << "The Base Object pointed is an A type" << std::endl;
	else if(dynamic_cast<B*>(p) != NULL)
		std::cout << "The Base Object pointed is an B type" << std::endl;
	else if(dynamic_cast<C*>(p) != NULL)
		std::cout << "The Base Object pointed is an C type" << std::endl;
	else
		std::cout << "Unknown type" << std::endl;

}

void identify(Base& p)
{
	try
	{
		(void)dynamic_cast<A&>(p);
		std::cout << "The Base Object refernce is an A type" << std::endl;
		return;
	}
	catch(...)
	{
		
	}
	try
	{
		(void)dynamic_cast<B&>(p);
		std::cout << "The Base Object refernce is an B type" << std::endl;
		return;
	}
	catch(...)
	{

	}
	try
	{
		(void)dynamic_cast<C&>(p);
		std::cout << "The Base Object refernce is an C type" << std::endl;
		return;
	}
	catch(...)
	{

	}
	std::cout << "Unknown type" << std::endl;

}


int main()
{

    Base *p = generate();

    identify(p);
    identify(*p);
    
    delete p;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyriarte <jyriarte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:00:35 by jyriarte          #+#    #+#             */
/*   Updated: 2025/06/10 14:29:49 by jyriarte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <cstdlib>

Base *generate(void);
void identify(Base *p);
void identify(Base &p);

int main(void)
{
    Base *base = generate();
    identify(base);
    identify(*base);
    delete (base);
    return (0);
}

Base *generate(void)
{
    std::srand(time(0));
    switch (std::rand() % 3)
    {
    case 0:
        return (new A());
        break;
    case 1:
        return (new B());
        break;
    default:
        return (new C());
    }
}

void identify(Base *p)
{
    if (!p)
        return;

    A *a;
    a = dynamic_cast<A *>(p);
    if (a)
    {
        std::cout << "A" << std::endl;
        return;
    }

    B *b;
    b = dynamic_cast<B *>(p);
    if (b)
    {
        std::cout << "B" << std::endl;
        return;
    }

    C *c;
    c = dynamic_cast<C *>(p);
    if (c)
    {
        std::cout << "C" << std::endl;
        return;
    }
}

void identify(Base &p)
{
    try
    {
        A a = dynamic_cast<A &>(p);
        std::cout << "A" << std::endl;
        return;
    }
    catch (std::exception &e)
    {
    }
    try
    {
        B b = dynamic_cast<B &>(p);
        std::cout << "B" << std::endl;
        return;
    }
    catch (std::exception &e)
    {
    }
    try
    {
        C c = dynamic_cast<C &>(p);
        std::cout << "C" << std::endl;
        return;
    }
    catch (std::exception &e)
    {
    }
}

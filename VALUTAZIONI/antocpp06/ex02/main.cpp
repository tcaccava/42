/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:39:58 by apintaur          #+#    #+#             */
/*   Updated: 2025/08/14 10:39:59 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include <time.h>
#include <cstdlib>
#include "header.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

// Mini helper
static void sep(const char* title)
{
	std::cout << "\n====	" << title << " ====\n" << std::endl;
}

// Test 1: explicit objects, both overloads
static void test_explicit()
{
	sep("Explicit instances & overload resolution");

	A	a;
	B	b;
	C	c;

	std::cout << "identify(&a)	->	";
	identify(&a);
	std::cout << "identify(a)	->	";
	identify(a);

	std::cout << "identify(&b)	->	";
	identify(&b);
	std::cout << "identify(b)	->	";
	identify(b);

	std::cout << "identify(&c)	->	";
	identify(&c);
	std::cout << "identify(c)	->	";
	identify(c);

}

// Test 2: random generation, both overloads, type counts. Loop approach
static void test_random_series(int N)
{
	sep("Random series via generate() (pointer + reference)");

	Base** bag = new Base*[N];

	int countA = 0, countB = 0, countC = 0;

	for (int i = 0; i < N; ++i)
	{
		bag[i] = generate();

		// Identify through pointer overload
		std::cout << "[#" << i << "]	ptr	->	";
		identify(bag[i]);

		// Identify through reference overload
		std::cout << "[#" << i << "]	ref	->	";
		identify(*bag[i]);

		// Count via dynamic_cast (for a sanity check on distribution)
		if (dynamic_cast<A*>(bag[i])) ++countA;
		else if (dynamic_cast<B*>(bag[i])) ++countB;
		else if (dynamic_cast<C*>(bag[i])) ++countC;
	}

	std::cout << "\nType counts after " << N << " generations:" << std::endl;
	std::cout << "- A: " << countA << std::endl;
	std::cout << "- B: " << countB << std::endl;
	std::cout << "- C: " << countC << std::endl;

	// cleanup
	for (int i = 0; i < N; ++i)
		delete bag[i];
	delete [] bag;
}

// Test 3: polymorphic aliasing (Base refs/pointers bound to derived)
static void test_polymorphic_aliasing()
{
	sep("Polymorphic aliasing (Base& / Base* bound to derived) ");

	A		a;
	B		b;
	C		c;
	Base&	ra = a;
	Base&	rb = b;
	Base&	rc = c;
	Base*	pa = &a;
	Base*	pb = &b;
	Base*	pc = &c;

	std::cout << "identify(ra)	->	"; identify(ra);
	std::cout << "identify(rb)	->	"; identify(rb);
	std::cout << "identify(rc)	->	"; identify(rc);

	std::cout << "identify(pa)	->	"; identify(pa);
	std::cout << "identify(pb)	->	"; identify(pb);
	std::cout << "identify(pc)	->	"; identify(pc);
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(0)));

	test_explicit();
	test_polymorphic_aliasing();
	test_random_series(20);

	return 0;
}

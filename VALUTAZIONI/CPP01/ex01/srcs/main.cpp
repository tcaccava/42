#include <iostream>
#include "Zombie.hpp"

int	main()
{
	Zombie*	zm_array = zombieHorde(10, "PIPPUS");
	for (int i = 0; i < 10; i++)
		zm_array[i].announce();
	delete[] zm_array;
	return (0);
}

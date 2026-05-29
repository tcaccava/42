#include <iostream>
#include "Zombie.hpp"

int	main()
{
	Zombie*	zombie = newZombie( "Foo" );
	delete zombie;
	randomChump("Pippus");
	return (0);
}

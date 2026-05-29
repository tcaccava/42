#include "Zombie.hpp"

Zombie* zombieHorde( int N, std::string name )
{
	Zombie* zm_array;

	zm_array = new Zombie[N];
	if (!zm_array)
		return (NULL);
	for (int i = 0; i < N; i++)
		zm_array[i].set_name(name); 
	return (zm_array);
}

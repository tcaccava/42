#include "Zombie.hpp"

Zombie::Zombie(std::string name) : name(name)
{
	std::cout << "Created Zombie " << name << std::endl;
}

Zombie::~Zombie()
{
	std::cout << "Destroyed Zombie " << name << std::endl;
}

void	Zombie::announce( void )
{
	std::cout << this->name  << ": BraiiiiiiinnnzzzZ... " << std::endl;
}

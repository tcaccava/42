#include "Zombie.hpp"

Zombie::Zombie( )
{
	std::cout << "Created Zombie without name"<< std::endl;
}

Zombie::Zombie(std::string name) : name(name)
{
	std::cout << "Created Zombie " << name << std::endl;
}

Zombie::~Zombie( )
{
	std::cout << "Destroyed Zombie " << name << std::endl;
}

void	Zombie::announce( void )
{
	std::cout << this->name  << ": BraiiiiiiinnnzzzZ... " << std::endl;
}

void	Zombie::set_name( std::string name )
{
	this->name = name;
}

std::string	Zombie::get_name( void )
{
	return (this->name);
}

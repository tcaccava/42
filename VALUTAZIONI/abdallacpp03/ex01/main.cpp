#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

int	main(void)
{
	std::cout << "=== Test Construction/Destruction ===" << std::endl;
	{
		ScavTrap couz("Hakim");
		couz.attack("enemy");
		couz.guardGate();
		couz.takeDamage(100);
		couz.beRepaired(15);
	} // Destruction automatique ici
	std::cout << "\n=== Test Copy Constructor ===" << std::endl;
	{
		ScavTrap original("Original");
		ScavTrap copy(original); // Test du constructeur de copie
		copy.guardGate();
	}
	return (0);
}

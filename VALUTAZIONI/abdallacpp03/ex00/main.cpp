#include "ClapTrap.hpp"

int	main(void)
{
	std::cout << "=== CLAPTRAP TESTS ===" << std::endl;
	// Test basique
	ClapTrap robot("Hakim");
	robot.attack("ennemi");
	robot.takeDamage(5);
	robot.beRepaired(3);
	robot.attack("autre_ennemi");
	// Test limites
	std::cout << "\n--- Tests limites ---" << std::endl;
	ClapTrap robot2("Test");
	// Épuiser l'énergie (10 actions)
	for (int i = 0; i < 12; i++)
	{
		robot2.attack("cible");
	}
	// Robot détruit
	ClapTrap robot3("Victime");
	robot3.takeDamage(15);       // Plus de vie
	robot3.attack("impossible"); // Ne devrait pas marcher
	return (0);
}

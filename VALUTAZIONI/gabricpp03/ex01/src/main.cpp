/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:50:45 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 15:39:34 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

int main(void)
{
	// /*
	// ** TEST 1: Construction and Destruction Chaining
	// ** When creating a ScavTrap, constructors are called in this order:
	// ** 1. ClapTrap constructor
	// ** 2. ScavTrap constructor
	// */
	// std::cout << "=== Construction/Destruction Chaining ===" << std::endl;
	// std::cout << "\n1. Creating ScavTrap 'Guardian'" << std::endl;
	// ScavTrap	scav1("Guardian");
	// std::cout << "\n2. Creating ScavTrap 'Defender'" << std::endl;
	// ScavTrap	scav2("Defender");


	// /*
	// ** TEST 2: Method Overriding
	// ** ScavTrap has its own attack() method that OVERRIDES ClapTrap's attack().
	// ** When you call scav1.attack(), ScavTrap::attack() is called (not ClapTrap::attack()).
	// */
	// std::cout << "\n=== Testing ScavTrap Specific Attack ===" << std::endl;
	// scav1.attack("enemy");
	// scav2.attack("intruder");

	
	// /*
	// ** TEST 3: ScavTrap-Specific Method
	// ** guardGate() is a NEW method that doesn't exist in ClapTrap.
	// ** This demonstrates that derived classes can ADD new functionality.
	// ** Calling it twice on the same object shows the state tracking (already guarding).
	// */
	// std::cout << "\n=== Testing Gate Keeper Mode ===" << std::endl;
	// scav1.guardGate();
	// scav2.guardGate();

	// /*
	// ** TEST 4: Inherited Methods
	// ** ScavTrap inherits takeDamage() and beRepaired() from ClapTrap.
	// ** These methods work unchanged in ScavTrap.
	// ** This demonstrates INHERITANCE: derived classes get base class functionality.
	// **
	// ** scav1 starts with 100 HP:
	// ** - Takes 30 damage -> 70 HP
	// ** - Repairs 20 -> 90 HP
	// ** - Takes 50 damage -> 40 HP
	// */
	// std::cout << "\n=== Testing Inherited Methods ===" << std::endl;
	// scav1.takeDamage(30);
	// scav1.beRepaired(20);
	// scav1.takeDamage(50);

	
	// /*
	// ** TEST 5: Copy Constructor with Scope
	// ** Creates a copy of scav1 inside a nested scope { }.
	// ** Copy constructor is called: ScavTrap(const ScavTrap& other)
	// ** scav3 is an exact copy of scav1 (including current HP and guard state).
	// **
	// ** When the scope ends (at }), scav3 is automatically destroyed.
	// **   the destruction messages:
	// ** "ScavTrap destructor for Guardian..."
	// ** "ClapTrap Guardian has been destroyed!"
	// **
	// ** scav1 still exists after this scope (only scav3 was destroyed).
	// */
	// std::cout << "\n=== Testing Copy Constructor ===" << std::endl;
	// {
	// 	std::cout << "Creating copy of Guardian..." << std::endl;
	// 	ScavTrap	scav3(scav1);
	// 	scav3.attack("target scav3");
	// 	scav3.guardGate();
	// 	std::cout << "Exiting scope (scav3 will be destroyed)..." << std::endl;
	// }

	// /*
	// ** TEST 6: Assignment Operator
	// ** Both scav4 and scav2 already exist (constructed separately).
	// ** Assignment operator copies scav2's values into scav4.
	// ** After assignment: scav4 has scav2's name, HP, energy, etc.
	// **
	// ** COPY CONSTRUCTOR vs ASSIGNMENT OPERATOR:
	// ** Copy constructor: ScavTrap scav3(scav1);  <- creates NEW object
	// ** Assignment operator: scav4 = scav2;       <- copies between EXISTING objects
	// */
	// std::cout << "\n=== Testing Assignment Operator ===" << std::endl;
	// ScavTrap	scav4("Temporary");
	// scav4 = scav2;
	// scav4.attack("assigned target");


	// /*
	// ** TEST 7: ClapTrap Comparison
	// ** Creates a regular ClapTrap (not a ScavTrap) for comparison.
	// ** Shows the differences:
	// ** - ClapTrap attack message is different (no "fiercely")
	// ** - ClapTrap deals 0 damage (vs ScavTrap's 20)
	// ** - ClapTrap has 10 HP/10 Energy (vs ScavTrap's 100 HP/50 Energy)
	// ** - ClapTrap doesn't have guardGate() method
	// */
	// std::cout << "\n=== Testing ClapTrap (for comparison) ===" << std::endl;
	// ClapTrap	clap("BasicBot");
	// clap.attack("target BasicBot");
	// clap.takeDamage(5);
	// clap.beRepaired(3);

	// /*
	// ** TEST 8: Energy Depletion
	// ** ScavTrap starts with 50 energy points.
	// ** Each attack costs 1 energy.
	// ** After 15 attacks: first 50 succeed, rest fail with "no energy" message.
	// ** Demonstrates resource management inherited from ClapTrap.
	// */
	// std::cout << "\n=== Testing Energy Depletion ===" << std::endl;
	// ScavTrap	energyTest("EnergyTestBot");
	// for (int i = 0; i < 15; i++) {
	// 	energyTest.attack("EnergyTestBot Enemy");
	// }

	// /*
	// ** TEST 9: Death Scenario
	// ** Tests that dead robots cannot perform actions.
	// ** deathTest takes 100 damage (exactly its max HP) and dies.
	// ** Subsequent actions should all fail:
	// ** - attack() fails: "died and is not able to attack"
	// ** - beRepaired() fails: "died and is not able to repair itself"
	// ** - guardGate() works (no HP check, but probably should have one!)
	// */
	// std::cout << "\n=== Testing Death Scenario ===" << std::endl;
	// ScavTrap	deathTest("DeathBot");
	// deathTest.takeDamage(100);
	// deathTest.attack("DeathBot Enemy");
	// deathTest.beRepaired(10);
	// deathTest.guardGate();

	// /*
	// ** Automatic Destruction
	// ** When main() ends, all local objects are destroyed automatically.
	// ** Destruction order is REVERSE of creation order 
	// ** 1. deathTest destroyed (last created)
	// ** 2. energyTest destroyed
	// ** 3. clap destroyed
	// ** 4. scav4 destroyed
	// ** 5. scav2 destroyed
	// ** 6. scav1 destroyed (first created)
	// **
	// ** For each ScavTrap destruction, you'll see TWO messages:
	// ** - ScavTrap destructor message
	// ** - ClapTrap destructor message (called automatically after)
	// **
	// ** This demonstrates the destructor chain in inheritance.
	// */
	ClapTrap base;
	ScavTrap derivata;

	 std::cout << "\n=== Program Ending - Destructors Will Be Called ===" << std::endl;
	return (0);
}
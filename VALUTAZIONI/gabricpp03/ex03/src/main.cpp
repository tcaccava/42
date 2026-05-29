/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:50:00 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 17:12:47 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DiamondTrap.hpp"
#include <iostream>

void printHeader(const std::string& title) {
	std::cout << "\n===============================================" << std::endl;
	std::cout << "" << title << "" << std::endl;
	std::cout << "===============================================\n" << std::endl;
}

void	testBasicDiamondTrap() {
	printHeader("TEST 1: Basic DiamondTrap Creation and whoAmI");

	DiamondTrap	diamond("Destroyer");
	diamond.whoAmI();

	std::cout << "\nTesting inherited methods:" << std::endl;
	diamond.attack("Target1");
	diamond.takeDamage(30);
	diamond.beRepaired(20);

	std::cout << "\nTesting ScavTrap special ability:" << std::endl;
	diamond.guardGate();

	std::cout << "\nTesting FragTrap special ability:" << std::endl;
	diamond.highFivesGuys();
}

void	testDefaultConstructor() {
	printHeader("TEST 2: Default Constructor");

	DiamondTrap	defaultDiamond;
	defaultDiamond.whoAmI();
	defaultDiamond.attack("Enemy");
}

void	testCopyConstructor() {
	printHeader("TEST 3: Copy Constructor");

	DiamondTrap	original("Original");
	std::cout << "\nOriginal:" << std::endl;
	original.whoAmI();

	std::cout << "\nCreating copy:" << std::endl;
	DiamondTrap	copy(original);
	std::cout << "\nCopy:" << std::endl;
	copy.whoAmI();

	std::cout << "\nTesting both attack the same target:" << std::endl;
	original.attack("SharedEnemy");
	copy.attack("SharedEnemy");
}

void	testAssignmentOperator() {
	printHeader("TEST 4: Assignment Operator");

	DiamondTrap	first("First");
	DiamondTrap	second("Second");

	std::cout << "\nBefore assignment:" << std::endl;
	first.whoAmI();
	second.whoAmI();

	std::cout << "\nAssigning first to second:" << std::endl;
	second = first;

	std::cout << "\nAfter assignment:" << std::endl;
	second.whoAmI();
}

void	testMultipleDiamondTraps() {
	printHeader("TEST 5: Multiple DiamondTraps Interaction");

	DiamondTrap	warrior("Warrior");
	DiamondTrap	mage("Mage");
	DiamondTrap	archer("Archer");

	std::cout << "\nAll three identify themselves:" << std::endl;
	warrior.whoAmI();
	mage.whoAmI();
	archer.whoAmI();

	std::cout << "\nBattle simulation:" << std::endl;
	warrior.attack("Mage");
	mage.takeDamage(30);
	mage.attack("Archer");
	archer.takeDamage(30);
	archer.attack("Warrior");
	warrior.takeDamage(30);

	std::cout << "\nHealing phase:" << std::endl;
	warrior.beRepaired(15);
	mage.beRepaired(15);
	archer.beRepaired(15);
}

void	testAttributesInheritance() {
	printHeader("TEST 6: Verifying Attribute Inheritance");

	DiamondTrap	diamond("Hybrid");

	std::cout << "DiamondTrap should have:" << std::endl;
	std::cout << "- Hit points from FragTrap (100)" << std::endl;
	std::cout << "- Energy points from ScavTrap (50)" << std::endl;
	std::cout << "- Attack damage from FragTrap (30)" << std::endl;
	std::cout << "- Attack() method from ScavTrap" << std::endl;

	std::cout << "\nTesting with multiple attacks to verify energy (should have 50):" << std::endl;
	for (int i = 1; i <= 5; i++) {
		diamond.attack("TestTarget");
	}

	std::cout << "\nTesting damage threshold:" << std::endl;
	diamond.takeDamage(50);
	diamond.beRepaired(10);
	diamond.takeDamage(70);
	diamond.attack("ShouldNotWork");
}

void	testDestructorOrder() {
	printHeader("TEST 7: Destructor Call Order");

	std::cout << "Creating DiamondTrap in a scope..." << std::endl;
	{
		DiamondTrap	scoped("Scoped");
		scoped.whoAmI();
	}
	std::cout << "\nScoped DiamondTrap destroyed" << std::endl;
}

void	testEnergyDepletion() {
	printHeader("TEST 8: Energy Depletion Test");

	DiamondTrap energyTest("EnergyTester");

	std::cout << "DiamondTrap has 50 energy points (from ScavTrap)" << std::endl;
	std::cout << "Attempting 51 actions to deplete energy:\n" << std::endl;

	for (int i = 1; i <= 51; i++) {
		if (i <= 25) {
			std::cout << "Action " << i << ": ";
			energyTest.attack("Target");
		} 
		else if (i <= 50) {
			std::cout << "Action " << i << ": ";
			energyTest.beRepaired(1);
		}
		else {
			std::cout << "\nAction " << i << " (should fail - no energy): ";
			energyTest.attack("Target");
		}
	}
}

int	main() {
	// testBasicDiamondTrap();
	// testDefaultConstructor();
	// testCopyConstructor();
	// testAssignmentOperator();
	// testMultipleDiamondTraps();
	// testAttributesInheritance();
	// testDestructorOrder();
	// testEnergyDepletion();

	// printHeader("ALL TESTS COMPLETED");
	ScavTrap	prova("ciao");
	ScavTrap	target("target");
	prova.attack("target");

	FragTrap	prova1("ciao1");
	FragTrap	target1("target1");
	prova1.attack("target1");
	
	DiamondTrap	prova2("ciao2");
	DiamondTrap	target2("target2");
	prova2.attack("target2");

	return (0);
}

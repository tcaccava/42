/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:42:19 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 16:14:04 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"
#include <iostream>

int	main(void) {
	/*
	** TEST 1: Basic Creation and Attack
	** Creates a ClapTrap and performs basic attacks.
	** Each attack costs 1 energy point.
	*/
	std::cout << "\n=== TEST 1: Creation and basic attack ===" << std::endl;
	
	ClapTrap	robot1("Robot1");
	robot1.attack("Enemy1");
	robot1.attack("Enemy2");
	std::cout << std::endl;

	/*
	** TEST 2: Taking Damage
	** Tests the takeDamage() function.
	** Robot starts with 10 HP, takes 3 damage, then 5 more.
	** After: HP = 10 - 3 - 5 = 2
	*/
	std::cout << "\n=== TEST 2: Taking damage ===" << std::endl;
	robot1.takeDamage(3);
	robot1.takeDamage(5);
	std::cout << std::endl;

	/*
	** TEST 3: Repair System
	** Tests the beRepaired() function.
	** Each repair costs 1 energy point.
	** HP increases by repair amount (no maximum limit).
	*/
	std::cout << "\n=== TEST 3: Being repaired ===" << std::endl;
	robot1.beRepaired(4);
	robot1.beRepaired(2);
	std::cout << std::endl;

	/*
	** TEST 4: Energy Depletion
	** ClapTraps start with 10 energy points.
	** We've already used 4 (2 attacks + 2 repairs).
	** This loop uses the remaining 6, then tries 2 more (should fail).
	** Demonstrates resource management.
	*/
	std::cout << "\n=== TEST 4: Energy depletion ===" << std::endl;
	for (int i = 0; i < 8; i++)
		robot1.attack("Target");
	std::cout << std::endl;

	/*
	** TEST 5: Death Prevention
	** Tests that dead robots cannot perform actions.
	** Robot2 takes 15 damage (more than its 10 HP), dies.
	** Subsequent attack and repair attempts should fail with messages.
	*/
	std::cout << "\n=== TEST 5: Dead robot cannot act ===" << std::endl;
	ClapTrap	robot2("Robot2");
	robot2.takeDamage(15);
	robot2.attack("Someone");
	robot2.beRepaired(5);
	std::cout << std::endl;

	/*
	** TEST 6: Copy Constructor
	** Tests the copy constructor: ClapTrap robot4(robot3);
	** robot4 should be an exact copy of robot3.
	** They are separate objects (modifying one doesn't affect the other).
	** Copy constructor is called at creation time.
	*/
	std::cout << "\n=== TEST 6: Copy constructor ===" << std::endl;
	ClapTrap	robot3("Robot3");
	ClapTrap	robot4(robot3);
	robot4.attack("TestTarget");
	std::cout << std::endl;

	/*
	** TEST 7: Assignment Operator
	** Tests the assignment operator: robot6 = robot5;
	** Both objects already exist, assignment copies values.
	** robot6's old values are replaced with robot5's values.
	** Assignment operator is called after objects are created.
	*/
	std::cout << "\n=== TEST 7: Assignment operator ===" << std::endl;
	ClapTrap	robot5("Robot5");
	robot5.takeDamage(3);
	ClapTrap	robot6("Robot6");
	robot6 = robot5;
	
	std::cout << robot6.getHit() << std::endl;

	/*
	** Automatic Destruction
	** When main() ends, all local objects are destroyed automatically.
	** Destructors are called in reverse order of construction.
	** Watch the console for destruction messages.
	*/
	std::cout << "\n=== End of tests - Automatic destruction ===" << std::endl;
	return (0);
}
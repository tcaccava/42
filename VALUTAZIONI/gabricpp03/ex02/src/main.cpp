/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vhacman <vhacman@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:27:41 by vhacman           #+#    #+#             */
/*   Updated: 2025/10/27 16:03:42 by vhacman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScavTrap.hpp"
#include "FragTrap.hpp"

/*
** TEST 1: Base Class Testing
** Tests the original ClapTrap class to show the baseline behavior.
** This helps compare with the enhanced derived classes.
**
** ClapTrap stats: 10 HP, 10 Energy, 0 Attack
** Notice: attack() deals 0 damage (weak!)
*/
void	testClapTrap(void)
{
	std::cout << "\n=== Testing ClapTrap ===" << std::endl;
	ClapTrap	clap("Clappy");
	clap.attack("Clappy's target");   // ClapTrap version: simple message
	clap.takeDamage(5);                // 10 HP - 5 = 5 HP remaining
	clap.beRepaired(3);                // 5 HP + 3 = 8 HP
}

/*
** TEST 2: ScavTrap Testing
** Tests the first derived class from ex01.
**
** ScavTrap stats: 100 HP, 50 Energy, 20 Attack (much stronger than ClapTrap!)
** Key features:
** - OVERRIDES attack() with "fiercely attacks" message
** - Has guardGate() special ability
** - INHERITS takeDamage() and beRepaired() unchanged
*/
void	testScavTrap(void)
{
	std::cout << "\n=== Testing ScavTrap ===" << std::endl;
	ScavTrap	scav("Scavvy");
	scav.attack("Scavy's target");     // ScavTrap version: "fiercely attacks"
	scav.takeDamage(30);               // Inherited from ClapTrap
	scav.beRepaired(20);               // Inherited from ClapTrap
	scav.guardGate();                  // ScavTrap special ability (first time)
	scav.guardGate();                  // Test state tracking (already guarding)
}

/*
** TEST 3: FragTrap Testing
** Tests the new derived class introduced in ex02.
**
** FragTrap stats: 100 HP, 100 Energy, 30 Attack (strongest attack!)
** Key features:
** - Does NOT override attack() (uses ClapTrap's version)
** - Has highFivesGuys() special ability
** - INHERITS all ClapTrap methods unchanged
**
** IMPORTANT: Compare attack message with ScavTrap's version!
** FragTrap: "ClapTrap Fraggy attacks..." (inherited from ClapTrap)
** ScavTrap: "ScavTrap Scavvy fiercely attacks..." (overridden)
*/
void	testFragTrap(void)
{
	std::cout << "\n=== Testing FragTrap ===" << std::endl;
	FragTrap	frag("Fraggy");
	frag.attack("Fraggy's Target");    // Uses ClapTrap's attack (not overridden!)
	frag.takeDamage(40);               // Inherited from ClapTrap
	frag.beRepaired(25);               // Inherited from ClapTrap
	frag.highFivesGuys();              // FragTrap special ability
}

/*
** TEST 4: Construction and Destruction Chain
** Demonstrates the order of constructor and destructor calls in inheritance.
**
** CONSTRUCTION ORDER when creating FragTrap("Destroyer"):
** 1. ClapTrap("Destroyer") constructor
**    → Prints: "ClapTrap Destroyer has been called"
** 2. FragTrap constructor body
**    → Prints: "FragTrap destructor for Destroyer has been called"
**
** DESTRUCTION ORDER when frag goes out of scope:
** 1. ~FragTrap() destructor
**    → Prints: "FragTrap destructor for Destroyer has been called!"
** 2. ~ClapTrap() destructor (automatic)
**    → Prints: "ClapTrap Destroyer has been destroyed!"
*/
void	testConstructionDestruction(void)
{
	std::cout << "\n=== Testing Construction/Destruction Chaining ===" << std::endl;
	std::cout << "\n     Creating FragTrap " << std::endl;
	FragTrap	frag("Destroyer");
	std::cout << "\n    Destroying FragTrap (destructor chain test) " << std::endl;
	// frag is destroyed here when exiting function scope
}

/*
** TEST 5: Copy Constructor
** Tests the copy constructor: FragTrap(const FragTrap& other)
**
** FragTrap frag2(frag1) creates an exact copy of frag1.
** All attributes are copied: name, HP, energy, attack damage.
**
** CONSTRUCTION ORDER for frag2:
** 1. ClapTrap(frag1) copy constructor
**    → Copies inherited attributes from frag1's ClapTrap part
** 2. FragTrap copy constructor body
**    → FragTrap has no unique attributes to copy
**
** Both objects are independent - modifying one doesn't affect the other.
*/
void	testCopyConstructor(void)
{
	std::cout << "\n=== Testing Copy Constructor ===" << std::endl;
	FragTrap	frag1("Original");
	FragTrap	frag2(frag1);  // Copy constructor called here
	// Both frag1 and frag2 will be destroyed when function ends
}

/*
** TEST 6: Assignment Operator
** Tests the assignment operator: operator=(const FragTrap& other)
**
** frag2 = frag1 copies frag1's values into the already-existing frag2.
**
** DIFFERENCE FROM COPY CONSTRUCTOR:
** - Copy constructor: Creates NEW object as copy (FragTrap frag2(frag1))
** - Assignment operator: Copies between EXISTING objects (frag2 = frag1)
**
** Assignment calls:
** 1. ClapTrap::operator=(frag1) to copy inherited attributes
** 2. FragTrap::operator= body (nothing extra to copy for FragTrap)
*/
void	testAssignmentOperator(void)
{
	std::cout << "\n=== Testing Assignment Operator ===" << std::endl;
	FragTrap	frag1("First");
	FragTrap	frag2("Second");
	frag2 = frag1;  // Assignment operator called here
	// After assignment, frag2 has frag1's name and stats
}

/*
** TEST 7: Multiple Objects of Same Type
** Tests creating and using multiple FragTrap objects simultaneously.
** Demonstrates that each object maintains its own state independently.
**
** Shows:
** - Multiple objects can coexist
** - Each has its own name, HP, energy
** - Actions on one don't affect others
** - Destruction happens in reverse order of creation (stack)
*/
void	testMultipleFragTraps(void)
{
	std::cout << "\n=== Testing Multiple FragTraps ===" << std::endl;
	FragTrap	frag1("Alpha");    // Created first
	FragTrap	frag2("Beta");     // Created second
	FragTrap	frag3("Gamma");    // Created third

	frag1.highFivesGuys();
	frag2.highFivesGuys();
	frag3.highFivesGuys();

	frag1.attack("Beta");          // Alpha attacks Beta
	frag2.takeDamage(30);          // Beta takes damage
	frag2.beRepaired(15);          // Beta repairs itself
	// Destruction order: frag3, frag2, frag1 (reverse of creation)
}

/*
** TEST 8: Polymorphism  
** Demonstrates runtime polymorphism using base class pointers.
**
** POLYMORPHISM CONCEPT:
** Base class pointer can point to derived class objects.
** ClapTrap* can point to: ClapTrap, ScavTrap, or FragTrap objects.
**
** WHY VIRTUAL DESTRUCTOR IS CRITICAL:
** When we do: delete scav; (where scav is ClapTrap*)
** Without virtual destructor in ClapTrap:
**   - Only ~ClapTrap() would be called → MEMORY LEAK!
** With virtual destructor in ClapTrap:
**   - ~ScavTrap() is called first, then ~ClapTrap() → CORRECT!
**
** ATTACK METHOD BEHAVIOR:
** - clap->attack() calls ClapTrap::attack() (simple message)
** - scav->attack() calls ScavTrap::attack() (fiercely attacks) - would need virtual to work!
** - frag->attack() calls ClapTrap::attack() (FragTrap doesn't override)
*/
void	testPolymorphism(void)
{
	std::cout << "\n=== Testing Polymorphism ===" << std::endl;
	
	// Base class pointers pointing to derived class objects
	ClapTrap	*clap = new ClapTrap("PolyClap");  // ClapTrap → ClapTrap
	ClapTrap	*scav = new ScavTrap("PolyScav");  // ClapTrap → ScavTrap (polymorphism )
	ClapTrap	*frag = new FragTrap("PolyFrag");  // ClapTrap → FragTrap (polymorphism )

	// All three attacks will use ClapTrap::attack() because it's not virtual
	// If attack() were virtual, scav would use ScavTrap::attack()
	clap->attack("target1");
	scav->attack("target2");
	frag->attack("target3");

	// Deleting through base class pointer
	// Virtual destructor ensures proper cleanup!
	delete (clap);  // ~ClapTrap() only
	delete (scav);  // ~ScavTrap() then ~ClapTrap() (thanks to virtual!)
	delete (frag);  // ~FragTrap() then ~ClapTrap() (thanks to virtual!)
}

int	main(void)
{
	testClapTrap();
	testScavTrap();
	testFragTrap();
	testConstructionDestruction();
	testCopyConstructor();
	testAssignmentOperator();
	testMultipleFragTraps();
	testPolymorphism();
	return (0);
}
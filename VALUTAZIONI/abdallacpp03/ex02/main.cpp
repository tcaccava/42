#include "ClapTrap.hpp"
#include "ScavTrap.hpp"
#include "FragTrap.hpp"
#include <iostream>

int main()
{
    std::cout << "=== FragTrap Tests ===" << std::endl;
    
    // Test 1: Construction/Destruction
    std::cout << "\n--- Construction/Destruction ---" << std::endl;
    {
        FragTrap frag("Warrior");
        frag.attack("enemy");
        frag.highFivesGuys();
        frag.takeDamage(30);
        frag.beRepaired(15);
    }
    
    // Test 2: Copy constructor
    std::cout << "\n--- Copy Constructor ---" << std::endl;
    {
        FragTrap original("Original");
        FragTrap copy(original);
        copy.highFivesGuys();
    }
    
    // Test 3: Comparison with other classes
    std::cout << "\n--- All Classes Comparison ---" << std::endl;
    {
        ClapTrap basic("Basic");
        ScavTrap guard("Guard");
        FragTrap frag("Frag");
        
        basic.attack("target");  // 0 damage
        guard.attack("target");  // 20 damage
        frag.attack("target");   // 30 damage
        
        guard.guardGate();
        frag.highFivesGuys();
    }
    
    return 0;
}

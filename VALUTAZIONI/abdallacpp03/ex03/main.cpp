#include "ClapTrap.hpp"
#include "FragTrap.hpp"
#include "ScavTrap.hpp"
#include "DiamondTrap.hpp"
#include <iostream>

int main()
{
    std::cout << "=== DIAMONDTRAP TESTS ===" << std::endl;
    
    // Test 1: Construction/Destruction
    std::cout << "\n--- Test 1: Construction/Destruction ---" << std::endl;
    {
        DiamondTrap diamond("Hero");
        std::cout << "DiamondTrap created!" << std::endl;
    } // Vérifier l'ordre de destruction
    
    // Test 2: whoAmI function
    std::cout << "\n--- Test 2: whoAmI Function ---" << std::endl;
    {
        DiamondTrap diamond("Batman");
        diamond.whoAmI();
        
        DiamondTrap diamond2("Superman");  
        diamond2.whoAmI();
    }
    
    // Test 3: Attack function (should use ScavTrap's)
    std::cout << "\n--- Test 3: Attack Function ---" << std::endl;
    {
        DiamondTrap warrior("Warrior");
        warrior.attack("enemy");
        warrior.attack("another_enemy");
    }
    
    // Test 4: Special functions from both parents
    std::cout << "\n--- Test 4: Inherited Special Functions ---" << std::endl;
    {
        DiamondTrap hybrid("Hybrid");
        hybrid.guardGate();      // From ScavTrap
        hybrid.highFivesGuys();  // From FragTrap
        hybrid.whoAmI();         // Own function
    }
    
    // Test 5: Complete functionality test
    std::cout << "\n--- Test 5: Complete Functionality ---" << std::endl;
    {
        DiamondTrap ultimate("Ultimate");
        
        ultimate.whoAmI();
        ultimate.attack("vilain");
        ultimate.takeDamage(25);
        ultimate.beRepaired(10);
        ultimate.guardGate();
        ultimate.highFivesGuys();
        ultimate.whoAmI();
    }
    
    // Test 6: Default constructor
    std::cout << "\n--- Test 6: Default Constructor ---" << std::endl;
    {
        DiamondTrap defaultDiamond;
        defaultDiamond.whoAmI();
    }
    
    std::cout << "\n=== ALL DIAMONDTRAP TESTS COMPLETED ===" << std::endl;
    return 0;
}

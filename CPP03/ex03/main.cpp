#include "DiamondTrap.hpp"

int main() {
    std::cout << "=== DIAMOND TRAP EXERCISE 03 DEMONSTRATION ===" << std::endl;
    std::cout << std::endl;

    std::cout << "--- 1. COSTRUZIONE DIAMONDTRAP ---" << std::endl;
    DiamondTrap diamond("Warrior");
    std::cout << std::endl;

    std::cout << "--- 2. ATTACK METHOD ---" << std::endl;
    diamond.attack("Enemy");

    std::cout << "--- 3. WHOAMI FUNCTION  ---" << std::endl;
    diamond.whoAmI();
    std::cout << std::endl;

    std::cout << "--- 4. FUNZIONI SPECIALI DEI GENITORI ---" << std::endl;
    
    std::cout << "Funzione speciale di FragTrap:" << std::endl;
    diamond.highFivesGuy();
    
    std::cout << "Funzione speciale di ScavTrap:" << std::endl;
    diamond.guardGate();
    std::cout << std::endl;

    std::cout << "--- 5. TEST ALTRE FUNZIONI ---" << std::endl;
    std::cout << "takeDamage :" << std::endl;
    diamond.takeDamage(30);
    
    std::cout << "beRepaired :" << std::endl;
    diamond.beRepaired(15);
    std::cout << std::endl;

    std::cout << "--- 6. VERIFICA ATTRIBUTI ---" << std::endl;
    DiamondTrap diamond2("TestStats");
    std::cout << "Secondo il subject, DiamondTrap dovrebbe avere:" << std::endl;
    std::cout << "- Hit points: 100 (da FragTrap)" << std::endl;
    std::cout << "- Energy points: 50 (da ScavTrap)" << std::endl;  
    std::cout << "- Attack damage: 30 (da FragTrap)" << std::endl;
    
    std::cout << "Testiamo gli attributi attraverso le azioni:" << std::endl;
    diamond2.attack("TestTarget");
    diamond2.takeDamage(150);
    std::cout << std::endl;

    std::cout << "--- 7. TEST COPY CONSTRUCTOR E ASSIGNMENT OPERATOR ---" << std::endl;
    std::cout << "Copy constructor:" << std::endl;
    DiamondTrap diamond3(diamond);
    diamond3.whoAmI();
    
    std::cout << "Assignment operator:" << std::endl;
    DiamondTrap diamond4("TempName");
    diamond4 = diamond;
    diamond4.whoAmI();
    std::cout << std::endl;

    std::cout << "--- 8. DISTRUZIONE  ---" << std::endl;
    
    return 0;
}







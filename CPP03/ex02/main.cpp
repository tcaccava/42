#include "ClapTrap.hpp"
#include "FragTrap.hpp"

int main()
{
    std::cout << "=== TEST COSTRUTTORI ===" << std::endl;
    ClapTrap clap("BasicBot");
    FragTrap scav("GuardianBot");
    
    std::cout << "\n=== TEST VALORI INIZIALI ===" << std::endl;
    std::cout << "ClapTrap attack damage should be 0:" << std::endl;
    clap.attack("TestTarget");
    
    std::cout << "\nFragTrap attack damage should be 30:" << std::endl;
    scav.attack("TestTarget");
    
    std::cout << "\n=== TEST DIFFERENZE STATISTICHE ===" << std::endl;
    std::cout << "Testing ClapTrap (10 HP, 10 Energy):" << std::endl;
    clap.takeDamage(5);
    clap.takeDamage(6);
    clap.attack("Enemy");
    
    std::cout << "\nTesting FragTrap (100 HP, 50 Energy):" << std::endl;
    scav.takeDamage(30);
    scav.attack("Enemy");
    scav.beRepaired(15);
    
    std::cout << "\n=== TEST FUNZIONE SPECIALE ===" << std::endl;
    scav.highFivesGuy();
    
    std::cout << "\n=== TEST COPY CONSTRUCTOR ===" << std::endl;
    FragTrap scav_copy(scav);
    scav_copy.attack("CopyTest");
    
    std::cout << "\n=== TEST ASSIGNMENT OPERATOR ===" << std::endl;
    FragTrap scav_assigned("TempName");
    scav_assigned = scav;
    scav_assigned.attack("AssignTest");
    
    std::cout << "\n=== TEST ESAURIMENTO ENERGIA ===" << std::endl;
    FragTrap energy_test("EnergyBot");
    std::cout << "Testing energy depletion (50 attacks max):" << std::endl;
    
        for (int i = 1; i <= 52; i++) {
        if (i == 50 || i == 51 || i == 52) {
            std::cout << "Attack " << i << ": ";
        }
        energy_test.attack("Dummy");
        if (i == 50 || i == 51 || i == 52) {
            std::cout << std::endl;
        }
    }
    
    std::cout << "\n=== TEST MORTE E RESURREZIONE ===" << std::endl;
    FragTrap death_test("Phoenix");
    death_test.takeDamage(150); 
    death_test.attack("Ghost");   
    death_test.beRepaired(10);    
    death_test.highFivesGuy(); 
    
    std::cout << "\n=== FINE TEST ===" << std::endl;
    
    return 0;
}


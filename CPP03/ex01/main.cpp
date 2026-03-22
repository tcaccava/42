#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

int main()
{
    std::cout << "=== TEST COSTRUTTORI ===" << std::endl;
    ClapTrap clap("BasicBot");
    ScavTrap scav("GuardianBot");
    
    std::cout << "\n=== TEST VALORI INIZIALI ===" << std::endl;
    std::cout << "ClapTrap attack damage should be 0:" << std::endl;
    clap.attack("TestTarget");
    
    std::cout << "\nScavTrap attack damage should be 20:" << std::endl;
    scav.attack("TestTarget");
    
    std::cout << "\n=== TEST DIFFERENZE STATISTICHE ===" << std::endl;
    std::cout << "Testing ClapTrap (10 HP, 10 Energy):" << std::endl;
    clap.takeDamage(5);
    clap.takeDamage(6);
    clap.attack("Enemy");
    
    std::cout << "\nTesting ScavTrap (100 HP, 50 Energy):" << std::endl;
    scav.takeDamage(30);
    scav.attack("Enemy");
    scav.beRepaired(15);
    
    std::cout << "\n=== TEST FUNZIONE SPECIALE ===" << std::endl;
    scav.guardGate();
    
    std::cout << "\n=== TEST COPY CONSTRUCTOR ===" << std::endl;
    ScavTrap scav_copy(scav);
    scav_copy.attack("CopyTest");
    
    std::cout << "\n=== TEST ASSIGNMENT OPERATOR ===" << std::endl;
    ScavTrap scav_assigned("TempName");
    scav_assigned = scav;
    scav_assigned.attack("AssignTest");
    
    std::cout << "\n=== TEST ESAURIMENTO ENERGIA ===" << std::endl;
    ScavTrap energy_test("EnergyBot");
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
    ScavTrap death_test("Phoenix");
    death_test.takeDamage(150); 
    death_test.attack("Ghost");   
    death_test.beRepaired(10);    
    death_test.guardGate(); 
    
    std::cout << "\n=== FINE TEST ===" << std::endl;
    
    return 0;
}


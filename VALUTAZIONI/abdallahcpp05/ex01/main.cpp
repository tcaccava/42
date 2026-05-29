#include "Bureaucrat.hpp"
#include "Form.hpp"

int main() {
    std::cout << "=== TEST FORM + BUREAUCRAT ===" << std::endl;
    
    // Test 1 : Signature réussie avec operator
    std::cout << "\n--- Test signature réussie ---" << std::endl;
    try {
        Bureaucrat chef("Chef", 30);
        Form demande("Demande congés", 50, 40);
        
        std::cout << "Avant: " << demande << std::endl;  // ← Test operator
        chef.signForm(demande);
        std::cout << "Après: " << demande << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    // Test 2 : Signature échouée
    std::cout << "\n--- Test signature échouée ---" << std::endl;
    try {
        Bureaucrat stagiaire("Bob", 100);
        Form contrat("Contrat important", 20, 10);
        
        std::cout << "Avant: " << contrat << std::endl;
        stagiaire.signForm(contrat);
        std::cout << "Après: " << contrat << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    // Test 3 : Grades invalides à la création
    std::cout << "\n--- Test grades invalides ---" << std::endl;
    try {
        Form impossible("Impossible", 0, 200);  // Grades invalides
    }
    catch (std::exception &e) {
        std::cout << "Erreur création form: " << e.what() << std::endl;
    }
    
    return 0;
}

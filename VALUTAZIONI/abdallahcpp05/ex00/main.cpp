#include "Bureaucrat.hpp"

int main() {
    std::cout << "=== TEST BUREAUCRAT ===" << std::endl;
    
    std::cout << "\n--- Test création normale ---" << std::endl;
    try {
        Bureaucrat bob("Bob", 50);
        std::cout << "Création réussie: " << bob << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Erreur: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test grade trop haut ---" << std::endl;
    try {
        Bureaucrat boss("Boss", 0);
        std::cout << boss << std::endl;
    }
    catch (Bureaucrat::GradeTooHighException &e) {
        std::cout << "Erreur attrapée: " << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Autre erreur: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test grade trop bas ---" << std::endl;
    try {
        Bureaucrat intern("Intern", 151);
        std::cout << intern << std::endl;
    }
    catch (Bureaucrat::GradeTooLowException &e) {
        std::cout << "Erreur attrapée: " << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Autre erreur: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test increment/decrement ---" << std::endl;
    try {
        Bureaucrat alice("Alice", 2);
        std::cout << "Avant: " << alice << std::endl;
        
        alice.incrementGrade();
        std::cout << "Après increment: " << alice << std::endl;
        
        alice.incrementGrade();
        std::cout << "Après 2ème increment: " << alice << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Erreur lors de l'increment: " << e.what() << std::endl;
    }
    
    std::cout << "\n--- Test decrement limite ---" << std::endl;
    try {
        Bureaucrat lowest("Lowest", 149);
        std::cout << "Avant: " << lowest << std::endl;
        
        lowest.decrementGrade();
        std::cout << "Après decrement: " << lowest << std::endl;
        
        lowest.decrementGrade();
        std::cout << "Après 2ème decrement: " << lowest << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "Erreur lors du decrement: " << e.what() << std::endl;
    }
    
    return 0;
}

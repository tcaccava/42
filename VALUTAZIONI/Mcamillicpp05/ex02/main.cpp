#include <iostream>
#include <ctime>
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

int main() {
    // Seed random for RobotomyRequestForm
    std::srand(static_cast<unsigned>(std::time(NULL)));

    std::cout << "=== Test ShrubberyCreationForm with insufficient execution grade ===" << std::endl;
    {
        Bureaucrat john("John", 140);
        ShrubberyCreationForm form("home");
        john.signForm(form);
        john.executeForm(form);
    }

    std::cout << "\n=== Test ShrubberyCreationForm with sufficient grade ===" << std::endl;
    {
        Bureaucrat alice("Alice", 1);
        ShrubberyCreationForm form("garden");
        alice.signForm(form);
        alice.executeForm(form);
    }

    std::cout << "\n=== Test RobotomyRequestForm outcomes ===" << std::endl;
    {
        Bureaucrat bob("Bob", 1);
        RobotomyRequestForm form("Marvin");
        bob.signForm(form);
        for (int i = 0; i < 3; ++i) {
            bob.executeForm(form);
        }
    }

    std::cout << "\n=== Test PresidentialPardonForm ===" << std::endl;
    {
        Bureaucrat claire("Claire", 1);
        PresidentialPardonForm form("Trillian");
        claire.signForm(form);
        claire.executeForm(form);
    }

    std::cout << "\n=== Test ShrubberyCreationForm file creation failure ===" << std::endl;
    {
        Bureaucrat admin("Admin", 1);
        // Invalid path to trigger file creation exception
        ShrubberyCreationForm badForm("/invalid_path");
        try {
            admin.signForm(badForm);
            admin.executeForm(badForm);
        } catch (const std::exception& e) {
            std::cerr << "Expected error: " << e.what() << std::endl;
        }
    }

    return 0;
}

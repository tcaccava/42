#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main()
{
    std::cout << "=== TEST 1: Creazione Form valido ===" << std::endl;
    try
    {
        Form f1("Tax Form", 50, 25);
        std::cout << f1;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 2: Form con grade troppo alto (0) ===" << std::endl;
    try
    {
        Form f2("Invalid Form", 0, 25);
        std::cout << f2;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 3: Form con grade troppo basso (151) ===" << std::endl;
    try
    {
        Form f3("Invalid Form", 50, 151);
        std::cout << f3;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 4: Bureaucrat firma Form con successo ===" << std::endl;
    try
    {
        Bureaucrat bob("Bob", 40);
        Form contract("Contract", 50, 25);
        std::cout << bob;
        std::cout << contract;
        bob.signForm(contract);
        std::cout << contract;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 5: Bureaucrat NON riesce a firmare (grade troppo basso) ===" << std::endl;
    try
    {
        Bureaucrat intern("Intern", 100);
        Form topSecret("Top Secret", 10, 5);
        std::cout << intern;
        std::cout << topSecret;
        intern.signForm(topSecret);
        std::cout << topSecret;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 6: Firma al limite esatto ===" << std::endl;
    try
    {
        Bureaucrat exact("Exact", 50);
        Form borderline("Borderline Form", 50, 50);
        std::cout << exact;
        std::cout << borderline;
        exact.signForm(borderline);
        std::cout << borderline;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 7: Copy constructor Form ===" << std::endl;
    try
    {
        Form original("Original", 30, 20);
        Form copy(original);
        std::cout << "Original: " << original;
        std::cout << "Copy: " << copy;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

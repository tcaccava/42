#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include <iostream>

int main() {
	try {
		Bureaucrat boss("Alice", 1);
		Intern someRandomIntern;

		AForm* form1 = someRandomIntern.makeForm("shrubbery creation", "Garden");
		AForm* form2 = someRandomIntern.makeForm("robotomy request", "Bender");
		AForm* form3 = someRandomIntern.makeForm("presidential pardon", "Fry");
		AForm* form4 = someRandomIntern.makeForm("invalid form", "Nobody"); // Should return NULL or throw

		std::cout << "\n--- Signing and Executing Forms ---\n" << std::endl;

		if (form1) {
			boss.signForm(*form1);
			boss.executeForm(*form1);
			delete form1;
		}
		if (form2) {
			boss.signForm(*form2);
			boss.executeForm(*form2);
			delete form2;
		}
		if (form3) {
			boss.signForm(*form3);
			boss.executeForm(*form3);
			delete form3;
		}
		if (form4) {
			std::cout << "ERROR: form4 should not be created!" << std::endl;
			delete form4;
		}

	} catch (std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
	return 0;
}

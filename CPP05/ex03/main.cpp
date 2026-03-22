#include "AForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "Intern.hpp"

int main() {
    Intern a;
    AForm* form;
    AForm* wrongform;
    form = a.makeForm("robotomy request", "Bender");
    wrongform = a.makeForm("stocazzo","ciaone");
    std::cout << *wrongform << std::endl;
}

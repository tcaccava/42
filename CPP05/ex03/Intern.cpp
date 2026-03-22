#include "Intern.hpp"
#include "PresidentialPardonForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"

Intern::Intern() {}

Intern::~Intern() {}

Intern::Intern(const Intern& other) {}

Intern &Intern::operator=(const Intern& other){
    (void) other;
    return *this;
}

AForm* createRobotomy(const std::string target) {
    return (new RobotomyRequestForm(target));
} 

AForm* createPresidentialPardon(const std::string target){
    return (new PresidentialPardonForm(target));
}

AForm* createShrubbery(const std::string target) {
    return new ShrubberyCreationForm(target);
}

AForm* Intern::makeForm(const std::string name, const std::string target) {
    std::string names[3] = {"shrubbery creation", "robotomy request", "presidential pardon"};
    AForm* (*FormCreator[3])(const std::string) = { createShrubbery,createRobotomy,createPresidentialPardon};
    for(int i = 0; i < 3; i++){
        if(name == names[i]){
            AForm* form = FormCreator[i](target);
            std::cout << "Intern creates " << name << std::endl;
            return form;
        }
    }
    std::cerr << "Intern, unknown form : " << name << std::endl; //metti un throw per evitare segfault
   return NULL;
}
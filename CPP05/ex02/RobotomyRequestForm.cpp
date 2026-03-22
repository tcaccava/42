#include "RobotomyRequestForm.hpp"

RobotomyRequestForm::RobotomyRequestForm(const std::string _target) : AForm("RobotomyRequestForm", 72, 45, _target) {
}

RobotomyRequestForm::~RobotomyRequestForm() {} 

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& other) :AForm(other) {}

RobotomyRequestForm &RobotomyRequestForm::operator=(const RobotomyRequestForm& other) {
    if(this != &other)
        AForm::operator=(other);
    return *this;
}

void RobotomyRequestForm::action() const {
    std::cout << "BZZZZZZZ....Vrrrrrrrrrr" << std::endl;
    if(rand() % 2 == 0)
        std::cout << getTarget() << " has been successfully robotomized" << std::endl;
    else 
        std::cout << "Robotomy failed on " << getTarget() << std::endl;

}
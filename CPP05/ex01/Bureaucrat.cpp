#include "Bureaucrat.hpp"
#include <iostream>
#include "Form.hpp"

Bureaucrat::Bureaucrat(const std::string& n, int g) : name(n) {
    if (g < 1)
        throw GradeTooHighException();
    if(g > 150)
        throw GradeTooLowException();
    grade = g;
}

Bureaucrat::~Bureaucrat() {}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : name(other.name) {
    this->grade = other.grade;
}

Bureaucrat &Bureaucrat::operator=(const Bureaucrat& other) {
    if(this != &other)
        this->grade = other.grade;
    return *this;
}

std::string Bureaucrat::getName()const {
    return name;
}

int Bureaucrat::getGrade() const {
    return grade;
}

const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "grade too high";
}

const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "grade too low";
}

void Bureaucrat::increment() {
    grade--;
    if(grade < 1)
        throw GradeTooHighException();
}

void Bureaucrat::decrement() {
    grade++;
    if(grade > 150)
        throw GradeTooLowException();
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& burocrat) {
    os << burocrat.getName() << ", bureaucrat grade " << burocrat.getGrade() << ".";
    return os;
}

void Bureaucrat::signForm(Form &form) {
    try {
        form.beSigned(*this);
        std::cout << name << " signed " << form.getName() << std::endl;
    } catch (std::exception &e) {
        std::cout << name << " couldn't sign " << form.getName()
                  << " because " << e.what() << std::endl;
    }
}

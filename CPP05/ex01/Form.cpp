#include "Form.hpp"

Form::Form(const std::string _name,const int _grade_to_sign,const int _grade_to_exec) : name(_name), grade_to_sign(_grade_to_sign),grade_to_exec(_grade_to_exec) {
    if(grade_to_sign < 1)
        throw GradeTooHighException();
    if(grade_to_sign > 150)
        throw GradeTooLowException();
    if(grade_to_exec < 1)
        throw GradeTooHighException();
    if(grade_to_exec > 150)
        throw GradeTooLowException();
    is_signed = false;
}

Form::~Form() {}

Form::Form(const Form& other) : name(other.name), grade_to_sign(other.grade_to_sign), grade_to_exec(other.grade_to_exec){
    this->is_signed = other.is_signed;
}

Form &Form::operator=(const Form& other){
    if(this != &other)
        this->is_signed = other.is_signed;
    return *this;
}

std::string Form::getName() const {
    return name;
}

bool Form::isSigned() const {
    return is_signed;
}
        
int Form::getGradeToSign() const {
    return grade_to_sign;
}
        
int Form::getGradeToExec() const {
    return grade_to_exec;
}

void Form::beSigned(const Bureaucrat &burocrat) {
    if (burocrat.getGrade() <= grade_to_sign)
        is_signed = true;
    else 
        throw GradeTooLowException();
}

const char* Form::GradeTooHighException::what() const throw() {
    return ("grade is too high");
}

const char* Form::GradeTooLowException::what() const throw() {
    return ("grade is too low");
}

std::ostream& operator<<(std::ostream& os, const Form& form) {
    os << form.getName() << " , grade to sign " << form.getGradeToSign() << " , grade to exec " << form.getGradeToExec() << ".";
    return os;
}


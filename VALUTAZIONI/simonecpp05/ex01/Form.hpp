#pragma once

#include <ostream>
#include <string>
#include <iostream>

class Bureaucrat;

class Form
{
    public:
        Form();
        Form(std::string name, const unsigned int sign_grade, const unsigned int exec_grade);
        Form(const Form& other);
        Form &operator=(const Form& other);
        ~Form();

        class GradeTooHighException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class GradeTooLowException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        void beSigned(const Bureaucrat& other);
        std::string getName() const;
        bool isSigned() const;
        unsigned int getSignGrade() const;
        unsigned int getExecGrade() const;
    private:
        const std::string _name;
        bool _sign;
        const unsigned int _sign_grade;
        const unsigned int _exec_grade;
};

std::ostream &operator<<(std::ostream &os, const Form& other);

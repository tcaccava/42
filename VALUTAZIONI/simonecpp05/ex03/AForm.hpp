#pragma once
#include <ostream>
#include <string>
#include <iostream>

class Bureaucrat;

class AForm
{
    public:
        AForm();
        AForm(std::string name, const unsigned int sign_grade, const unsigned int exec_grade);
        AForm(const AForm& other);
        AForm &operator=(const AForm& other);
        virtual ~AForm();

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
        class FormNotSignedException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        void beSigned(const Bureaucrat& other);
        std::string getName() const;
        bool isSigned() const;
        unsigned int getSignGrade() const;
        unsigned int getExecGrade() const;

        void execute(Bureaucrat const & executor) const;
        virtual void executeAction() const = 0;

    private:
        const std::string _name;
        bool _sign;
        const unsigned int _sign_grade;
        const unsigned int _exec_grade;
};

std::ostream &operator<<(std::ostream &os, const AForm& other);

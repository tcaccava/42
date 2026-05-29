#pragma once

#include <ostream>
#include <string>
#include <iostream>

class Bureaucrat
{
    public:
        Bureaucrat();
        Bureaucrat(std::string name);
        Bureaucrat(unsigned int grade);
        Bureaucrat(std::string name, unsigned int grade);
        Bureaucrat(const Bureaucrat& other);
        Bureaucrat &operator=(const Bureaucrat& other);
        // Bureaucrat &operator<<(const Bureaucrat& other);
        ~Bureaucrat();

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

        std::string getName() const;
        unsigned int getGrade() const;
        void incrementGrade();
        void decrementGrade();
    private:
        const std::string _name;
        unsigned int _grade;
};

std::ostream &operator<<(std::ostream &os, const Bureaucrat& other);

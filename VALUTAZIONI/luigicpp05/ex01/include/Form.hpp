#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <Bureaucrat.hpp>

class Form
{
    private:
        std::string const   _name;
        bool                _signed;
        int const           _gradeToSign;
        int const           _gradeToExec;

    public:
        Form();
        Form(std::string const &name, int gradeToSign, int gradeToExec);
        ~Form();
        Form(Form const &src);
        Form& operator=(Form const &src);

        void                beSigned(Bureaucrat& bureaucrat);

        std::string const   &getName() const;
        bool                getSigned() const;
        int                 getGradeToSign() const;
        int                 getGradeToExec() const;

        class GradeTooLowException : public std::exception
        {
            public:
                const char *what() const throw();
        };

        class GradeTooHighException : public std::exception
        {
            public:
                const char *what() const throw();
        };
};

std::ostream &operator<<(std::ostream &os, Form const &src);

#endif

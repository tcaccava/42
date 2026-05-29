#ifndef AAForm_HPP
#define AAForm_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <Bureaucrat.hpp>

class AForm
{
    private:
        std::string const   _name;
        bool                _signed;
        int const           _gradeToSign;
        int const           _gradeToExec;

    public:
        AForm(std::string const &name, int gradeToSign, int gradeToExec);
        virtual ~AForm();
        AForm(AForm const &src);
        AForm& operator=(AForm const &src);

        void                beSigned(Bureaucrat const& bureaucrat);

        std::string const   &getName() const;
        bool                getSigned() const;
        int                 getGradeToSign() const;
        int                 getGradeToExec() const;

        class GradeTooLowException : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };

        class GradeTooHighException : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        virtual void execute(Bureaucrat const& executor) = 0;
        //virtual void do_something() = 0;
};

std::ostream &operator<<(std::ostream &os, AForm const &src);

#endif


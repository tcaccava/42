#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class AForm; 

class Bureaucrat
{
    private:
        std::string const   _name;
        int                 _grade;
    
    public:
        Bureaucrat();
        Bureaucrat(std::string const &name, int grade);
        ~Bureaucrat();
        Bureaucrat(Bureaucrat const &src);
        Bureaucrat&         operator=(Bureaucrat const &src);
        std::string const   &getName() const;
        int                 getGrade() const;
        void		        incrementGrade();
        void		        decrementGrade();

        void                signForm(AForm &form);

        class GradeTooHighException : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };


        class GradeTooLowException : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
};

//l'operator overload è esterno alla classe perchè accetta un ostream
std::ostream& operator<<(std::ostream& os, const Bureaucrat& src);


#endif
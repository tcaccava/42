#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class Bureaucrat
{
    private:
        std::string const _name;
        int               _grade;  /*1 is the highest, 150 is the lowest*/
    
    public:
        Bureaucrat();
        Bureaucrat(std::string const &name, int grade);
        ~Bureaucrat();
        Bureaucrat(Bureaucrat const &src);
        
        Bureaucrat&         operator=(Bureaucrat const &src);
        std::string const&  getName() const;
        int                 getGrade() const;
        void	        	incrementGrade();
        void	        	decrementGrade();
        
        class GradeTooHighException : public std::exception
        {
            public:
                const char *what() const throw(); //the keyword throw is the way std++98 makes "what()" avoiding to launch exceptions.       
        };

        class GradeTooLowException : public std::exception
        {
            public:
                const char *what() const throw();
        };
};

std::ostream& operator<<(std::ostream& os, const Bureaucrat& src);

#endif
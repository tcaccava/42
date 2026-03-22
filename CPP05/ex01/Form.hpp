#ifndef FORM_HPP
#define FORM_HPP

#include <string>
#include "Bureaucrat.hpp"
#include <iostream>

class Form {
    public:
        Form(const std::string _name,const int _grade_to_sign,const int _grade_to_exec);
        ~Form();
        Form(const Form& other);
        Form &operator=(const Form& other);

        std::string getName() const;
        bool isSigned() const;
        int getGradeToSign() const;
        int getGradeToExec() const;

        void beSigned(const Bureaucrat &burocrat);

        class GradeTooHighException : public std::exception {
            public :
                virtual const char* what() const throw();
        };

        class GradeTooLowException :public std::exception {
            public:
                virtual const char* what() const throw();
        };
    
    private :
        const std::string name;
        bool is_signed;
        const int grade_to_sign;
        const int grade_to_exec;
};

std::ostream& operator<<(std::ostream &os, const Form& form);

#endif
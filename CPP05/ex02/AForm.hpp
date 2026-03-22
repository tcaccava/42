#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>
#include <string>
#include "Bureaucrat.hpp"
#include <fstream>


class AForm
{
public:
    AForm(const std::string _name,const int to_sign,const int to_exec, const std::string &target);
    virtual ~AForm();
    AForm(const AForm &other);
    AForm &operator=(const AForm &other);

    std::string getName() const;
    int getGradeToSign() const;
    int getGradeToExec() const;
    std::string getTarget() const;
    bool isSigned() const;
    void setSigned() ;
    void beSigned(const Bureaucrat &burocrat);
    void execute(Bureaucrat const& executor) const;
    virtual void action() const = 0;

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

    class NotSignedException : public std::exception {
        public:
            virtual const char* what() const throw();
    };
private:
    const std::string name;
    const int to_exec;
    const int to_sign;
    bool is_signed;
    std::string target;
};

std::ostream& operator<<(std::ostream &os, const AForm& form);

#endif
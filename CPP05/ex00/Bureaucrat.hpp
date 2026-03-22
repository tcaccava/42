#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>

class Bureaucrat {
    public:
        class GradeTooHighException : public std::exception {
            public:
                virtual const char* what() const throw();
        };
        class GradeTooLowException : public std::exception {
            public:
                virtual const char* what() const throw();
        };
        Bureaucrat(const std::string& n,int g);
        ~Bureaucrat();
        Bureaucrat(const Bureaucrat& other);
        Bureaucrat &operator=(const Bureaucrat& other);

        std::string getName() const;
        int getGrade() const;
        void increment();
        void decrement();

    private:
        const std::string name;
        int grade; 
};

std::ostream& operator<<(std::ostream& os, const Bureaucrat& burocrat);

#endif

#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>

class Form;

class Bureaucrat
{
	private:
		const std::string _name;
		int _grade;
	public:
		Bureaucrat();
		Bureaucrat(std::string name, int grade);
		Bureaucrat(Bureaucrat &copy);
		virtual ~Bureaucrat();
		Bureaucrat &operator=( Bureaucrat &other);

		const std::string& getName()const;
		int getGrade()const;
		void incrementGrade();
		void decrementGrade();
		void signForm(Form &formular);
		class GradeTooHighException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Grade too high! Maximum = 1!");
				}
		};
		class GradeTooLowException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Grade too low! Minimum = 150!");
				}
		};
};

#endif

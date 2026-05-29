#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>

class Bureaucrat;

class Form
{
	private:
		const std::string _name;
		bool _signed;
		const int _gradeToSign;
		const int _gradeToExecute;
	
	public:
		Form();
		Form(const std::string &name, const int gradeToSign, const int gradeToExecute);
		Form(const Form &copy);
		virtual ~Form();
		Form& operator=(const Form& other);

		const std::string getName()const;
		bool isSigned()const;
		int getGradeToSign()const;
		int getGradeToExecute()const;

		void beSigned(const Bureaucrat &guy);
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

std::ostream& operator<<(std::ostream& os, const Form& form);

#endif

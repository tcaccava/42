#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>

class Bureaucrat;

class AForm
{
	private:
		const std::string _name;
		bool _signed;
		const int _gradeToSign;
		const int _gradeToExecute;
	
	public:
		AForm();
		AForm(const std::string &name, const int gradeToSign, const int gradeToExecute);
		AForm(const AForm &copy);
		virtual ~AForm();

		std::string getName()const;
		bool isSigned()const;
		int getGradeToSign()const;
		int getGradeToExecute()const;
		void beSigned(const Bureaucrat &guy);
		virtual void execute(Bureaucrat const &executor) const = 0;
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

std::ostream& operator<<(std::ostream& os, const AForm& form);

#endif

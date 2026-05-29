#pragma once

#include "Bureaucrat.hpp"

class Bureaucrat;

class Form
{
	public:
		Form(void);
		Form(std::string name, const int gradeToSign, const int gradeToExecute);
		Form(const Form& ref);
		Form& operator = (const Form& ref);
		~Form(void);

		std::string	getName(void) const;
		int			getGradeToSign(void) const;
		int			getGradeToExecute(void) const;
		bool		isItSigned(void) const;
		void		beSigned(const Bureaucrat& bureaucrat);

		class GradeTooHighException : public std::exception
		{
			public:
				const char* what() const throw();
		};
		class GradeTooLowException : public std::exception
		{
			public:
				const char* what() const throw();
		};

	private:
		bool				_signed;
		const std::string	_name;
		const int		 	_gradeToSign;
		const int		 	_gradeToExecute;
};

std::ostream& operator<< (std::ostream& os, const Form& ref);

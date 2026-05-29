#pragma once

#include "Bureaucrat.hpp"

class Bureaucrat;

class AForm
{
	public:
		AForm(void);
		AForm(std::string name, const int gradeToSign, const int gradeToExecute);
		AForm(const AForm& ref);
		AForm& operator = (const AForm& ref);
		virtual ~AForm(void);

		std::string		getName(void) const;
		int				getGradeToSign(void) const;
		int				getGradeToExecute(void) const;
		bool			isItSigned(void) const;
		void			beSigned(const Bureaucrat& bureaucrat);
		bool			beExecuted(const Bureaucrat& bureaucrat) const;
		virtual void	execute(Bureaucrat const & executor) const = 0;

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

std::ostream& operator<< (std::ostream& os, const AForm& ref);

#ifndef bureaucrat_HPP
# define bureaucrat_HPP

#include <iostream>
#include <string>
#include <exception>

class	Bureaucrat
{
	public:
		Bureaucrat(void);
		Bureaucrat(std::string name, int grade);
		Bureaucrat(const Bureaucrat& ref);
		Bureaucrat& operator = (const Bureaucrat& ref);
		~Bureaucrat(void);

		void		IncrementGrade(void);
		void		DecrementGrade(void);
		std::string	getName(void) const;
		int			getGrade(void) const;

		class GradeTooHighException: public std::exception
		{
			public:
				const char* what() const throw();
		};
		class GradeTooLowException: public std::exception
		{
			public:
				const char* what() const throw();
		};

	private:
		std::string const	_name;
		int					_grade;
};

std::ostream& operator<< (std::ostream& os, const Bureaucrat& ref);

#endif

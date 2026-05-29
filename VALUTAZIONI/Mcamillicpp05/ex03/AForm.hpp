#pragma once

#ifndef AForm_HPP
	#define AForm_HPP

#include "Bureaucrat.hpp"
#include <ctime>

class Bureaucrat;

class AForm
{
	private:
	const std::string _name;
	bool _signed;
	const int _sign_grade;
	const int _exe_grade;
	public:
	AForm(std::string name, int sign, int exe);
	AForm(const AForm &obj);
	AForm &operator =(const AForm &obj);
	virtual ~AForm();
	virtual void execute(Bureaucrat const &executor) const = 0;
	std::string getName() const;
	bool getIsSigned() const;
	int getSignGrade() const;
	int getExeGrade() const;
	void beSigned(const Bureaucrat &ins);
	bool executable(const Bureaucrat &ins) const;
	class GradeTooLowException : public std::exception
	{
		public:
		virtual const char *what() const throw();
	};
	class GradeTooHighException : public std::exception
	{
		public:
		virtual const char *what() const throw();
	};
	class NotSignedException : public std::exception
	{
			public:
			virtual const char* what() const throw();
	};
	class RobotomyFailed : public std::exception
	{
			public:
			virtual const char* what() const throw();
	};

};

std::ostream &operator <<(std::ostream &out, const AForm &obj);
#endif
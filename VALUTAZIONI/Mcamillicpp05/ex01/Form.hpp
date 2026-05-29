#pragma once

#ifndef FORM_HPP
	#define FORM_HPP

#include "Bureaucrat.hpp"

class Bureaucrat;

class Form
{
	private:
	const std::string _name;
	bool _signed;
	const int _sign_grade;
	const int _exe_grade;
	public:
	Form(std::string name, int sign, int exe);
	Form(const Form &obj);
	Form &operator =(const Form &obj);
	~Form();
	std::string getName() const;
	bool getIsSigned() const;
	int getSignGrade() const;
	int getExeGrade() const;
	void beSigned(const Bureaucrat &ins);
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

};

std::ostream &operator <<(std::ostream &out, const Form &obj);
#endif
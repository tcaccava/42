#pragma once
#ifndef BUREAUCRAT_HPP
  #define BUREAUCRAT_HPP

	#include <string>
	#include <iostream>
	#include <ctime>
	#include <cstdlib>
	#include "AForm.hpp"

class AForm;

class Bureaucrat
{
	private:
		const std::string _name;
		int _grade;
	public:
		~Bureaucrat();
		Bureaucrat(const std::string grade, int name); // const costringe a usare una lista di inizializzazione, piu pulita
		Bureaucrat(const Bureaucrat &ins);
		Bureaucrat& operator = (const Bureaucrat &ins); //equal op overload
	class GradeTooHighException : public std::exception //public per ereditare tutto la exeption class
	{
			public:
			virtual const char* what() const throw(); //si scvrive cosi perche dichiarato cosi in exeption, stiamo facendo un override, è virtuale dal padre ma lo mettiamo lo stesso per sicurezza

	};
	class GradeTooLowException : public std::exception
	{
			public:
			virtual const char* what() const throw();
	};
	int getGrade() const; //per esser chiamata in una funzione const bureaucrat &b
	std::string getName() const;
	void signForm(AForm &obj);
	void Promotion();
	void Demotion();
	void executeForm(AForm const & form);
};

std::ostream &operator <<(std::ostream &out, const Bureaucrat &ins); // fuori perché il primo operando implicito se fosse in una classe sarebbe della classe stessa, ma in questo caso è &out
#endif
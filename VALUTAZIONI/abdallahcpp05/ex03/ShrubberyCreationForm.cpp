#include "ShrubberyCreationForm.hpp"
#include <fstream> 

ShrubberyCreationForm::ShrubberyCreationForm() : AForm("Shrubbery Creation", 145, 137), _target("default")
{
	//std::cout << "Default constructor called ShrubberyCreationForm"<< std::endl;
}
ShrubberyCreationForm::ShrubberyCreationForm(std::string target) : AForm("Shrubbery Creation", 145, 137), _target(target)
{
	//std::cout << "Parameter constructor called ShrubberyCreationForm" << std::endl;
}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &copy) : AForm(copy), _target(copy._target)
{
	//std::cout << "Copy constructor called ShrubberyCreationForm" << std::endl;
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{
	//std::cout << "Destructor called ShrubberyCreationForm" << std::endl;
}

void ShrubberyCreationForm::execute(Bureaucrat const &executor) const
{
	if (!this->isSigned())
		throw std::runtime_error("Form not signed!");
	if (executor.getGrade() > this->getGradeToExecute())
		throw GradeTooLowException();
	std::ofstream file((_target + "_shrubbery").c_str());
	file << "this is a tree:" << std::endl;
	file << "    /\\ " << std::endl;
	file << "   /  \\" << std::endl;
	file << "  /    \\" << std::endl;
	file << " /      \\" <<std::endl;
	file << "/        \\" << std::endl;
	file << "----------" << std::endl;
	file << "    ||"     << std::endl;
	file << "    ||"     << std::endl;
	file << "    ||"     << std::endl;
	file << "    ||"     << std::endl;
	file.close();
}


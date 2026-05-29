#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm(void)  : AForm()
{}

ShrubberyCreationForm::ShrubberyCreationForm(std::string target) : AForm(target, 145, 137)
{}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& ref) : AForm(ref)
{}

ShrubberyCreationForm& ShrubberyCreationForm::operator = (const ShrubberyCreationForm& ref)
{
	(void) ref;
	return (*this);
}

ShrubberyCreationForm::~ShrubberyCreationForm(void)
{}

void ShrubberyCreationForm::execute(Bureaucrat const& executor) const
{
	try
	{
		beExecuted(executor);
		std::ofstream file(std::string(this->getName() + "_shrubbery").c_str());
		if (!file)
			std::cout << "Couldn't create file\n";
		else
		{
			std::cout << executor.getName() << " executed " << getName() << std::endl;
			file << "ASCII trees";
			file.close();
		}
	}
	catch(const std::exception& e)
	{
		std::cout << executor.getName() << " couldn't execute " << getName() << " because " << e.what() << std::endl;
	}
}
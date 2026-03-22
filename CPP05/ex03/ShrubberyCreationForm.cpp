#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm(std::string _target) : AForm("ShrubberyCreationForm", 145, 137, _target) {}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &other) : AForm(other) {}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(const ShrubberyCreationForm &other)
{
    if (this != &other)
        AForm::operator=(other);
    return *this;
    
}

void ShrubberyCreationForm::action() const
{
    std::string filename = this->getTarget() + "_shrubbery";
    std::ofstream file(filename.c_str());
    if (!file)
        throw std::runtime_error("Failed to create file");
    file << "        *        \n";
    file << "       /_\\       \n";
    file << "      /o_o\\      \n";
    file << "     /_/_\\_\\     \n";
    file << "    /o_o_o_o\\    \n";
    file << "   /_/_/_/_/_\\   \n";
    file << "        |        \n";
    file << "       / \\       \n";
}

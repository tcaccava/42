#include "ShrubberyCreationForm.hpp"
#include <iostream>
#include <fstream> 
#include <unistd.h>   //for sleep


ShrubberyCreationForm::ShrubberyCreationForm(const std::string target) : AForm(target , 145, 137){}
    

void ShrubberyCreationForm::execute(Bureaucrat const &executor)
{
    std::cout << "Bureaucrat " << executor.getName() << ": execution in progress ...\n" << std::flush;;
    sleep(1);
    if (!this->getSigned())
       throw  (GradeTooLowException());
    if (this->getGradeToExec() < executor.getGrade())
        throw (GradeTooLowException());
    //specific behaviour ...overloading


     std::string name;

    name.append(getName());
    name.append("_shrubbery");

    //std::cout << name << std::endl;
    std::ofstream file(name.c_str());   
    if (!file)
    {
        std::cerr << "File not created.\n";
        return;
    }

    file << "        ccee88oo\n";
    file << "  C8O8O8Q8PoOb o8oo\n";
    file << " dOB69QO8PdUOpugoO9bD\n";
    file << "CgggbU8OU qOp qOdoUOdcb\n";
    file << "     6OuU  /p u gcoUodpP\n";
    file << "         \\\\//  /douUP\n";
    file << "           \\\\\\////\n";
    file << "            |||/\n";
    file << "            |||\n";
    file << "            |||\n";
    file << "      .....//||||\\\\....\n";

    file.close();


    std::cout << "The bureucrat named " << executor.getName() << " has executed the form\n";
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShrubberyCreationForm.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcapalbo <mcapalbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:47:47 by mcapalbo          #+#    #+#             */
/*   Updated: 2025/03/28 14:18:56 by mcapalbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShrubberyCreationForm.hpp"
#include <fstream>

ShrubberyCreationForm::ShrubberyCreationForm(const std::string &target)
    : AForm("ShrubberyCreationForm", 145, 137), target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &other)
    : AForm(other), target(other.target) {}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(const ShrubberyCreationForm &other)
{
	if (this != &other)
	{
		AForm::operator=(other);
		target = other.target;
	}
	return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

void ShrubberyCreationForm::execute(Bureaucrat const &executor) const
{
	if (!isSigned())
		throw AForm::FormNotSignedException();
	if (executor.getGrade() > getGradeToExecute())
		throw AForm::GradeTooLowException();

	std::ofstream file((target + "_shrubbery").c_str());
	if (!file)
	{
		std::cerr << "Error: could not create file " << target + "_shrubbery" << std::endl;
		return;
	}
   file <<"          .     .  .      +     .      .          .         "<<std::endl;
				file <<"     .       .      .     #       .           .             "<<std::endl;
				file <<"        .      .         ###            .      .      .     "<<std::endl;
				file <<"      .      .   \"#:. .:##\"##:. .:#\"  .      .              "<<std::endl;
				file <<"       .     \"#:.    .:#\"###\"#:.    .:#\"  .        .       ."<<std::endl;
				file <<"  .             \"#########\"#########\"        .        .     "<<std::endl;
				file <<"        .    \"#:.  \"####\"###\"####\"  .:#\"   .       .        "<<std::endl;
				file <<"     .     .  \"#######\"\"##\"##\"\"#######\"                  .  "<<std::endl;
				file <<"                .\"##\"#####\"#####\"##\"           .      .     "<<std::endl;
				file <<"    .   \"#:. ...  .:##\"###\"###\"##:.  ... .:#\"     .         "<<std::endl;
				file <<"    .    .     \"#####\"\"#######\"\"#####\"    .      .          "<<std::endl;
				file <<"            .     \"      000      \"    .     .              "<<std::endl;
				file <<"       .         .   .   000     .        .       .          "<<std::endl;
				file <<".. .. ..................O000O........................ ......"<<std::endl;
				//file.close();
}
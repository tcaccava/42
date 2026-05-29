/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShrubberyCreationForm.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcapalbo <mcapalbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:09:50 by mcapalbo          #+#    #+#             */
/*   Updated: 2025/03/28 15:12:48 by mcapalbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm(std::string target) : AForm("ShrubberyCreationForm", 145, 137)
{
	_target = target;
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{
}

ShrubberyCreationForm::ShrubberyCreationForm(ShrubberyCreationForm const &src) : AForm(src)
{
	_target = src._target;
}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(ShrubberyCreationForm const &src)
{
	if (this == &src)
		_target = src._target;
	return *this;
}

void ShrubberyCreationForm::execute(Bureaucrat const &executor) const
{
	if (executor.getGrade() > this->getGradeToExecute())
		throw GradeTooLowException();
	if (this->getSigned() == false)
		throw FormNotSignedException();

	std::string filename = _target + "_shrubbery";
	std::ofstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file" << std::endl;
		return ;
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
				file.close();
}
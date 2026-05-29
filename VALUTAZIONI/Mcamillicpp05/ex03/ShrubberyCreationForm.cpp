#include "ShrubberyCreationForm.hpp"


ShrubberyCreationForm::ShrubberyCreationForm(std::string target) : AForm("ShrubberyCreationForm", 145, 137) , _target(target)
{

}
void ShrubberyCreationForm::execute(const Bureaucrat &executor) const
{
	if (this->getIsSigned() == false)
		throw(NotSignedException());
	if (!this->executable(executor))
		throw(GradeTooLowException());
	std::string nameFile;
	const char* trueNameFile;
	nameFile = this->_target + "_shrubery";
	trueNameFile = nameFile.c_str();
	std::ofstream file(trueNameFile);
	if (file.is_open())
	{
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
	else
		throw std::runtime_error("ShrubberyCreationForm: cannot create file");
}


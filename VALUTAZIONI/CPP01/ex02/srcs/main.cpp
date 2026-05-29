#include <iostream>
#include <string>

int	main( void )
{
	std::string		brain = "HI THIS IS BRAIN";
	std::string*	stringPTR = &brain;
	std::string&	stringREF = brain;
	
	std::cout << "memory address of string 'brain': " << &brain << std::endl;
	std::cout << "memory address of stringPTR:      " << stringPTR << std::endl;
	std::cout << "memory address of stringREF:      " << &stringREF << std::endl;
	
	std::cout << std::endl;

	std::cout << "value of string 'brain': " << brain << std::endl;
	std::cout << "value of stringPTR:      "<< *stringPTR << std::endl;
	std::cout << "value of stringREF:      "<< stringREF << std::endl;
	
	return (0);
}

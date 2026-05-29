#include "Harl.h"
#include "Harl.hpp"

void	Harl::debug( void )
{
	std::cout << YELLOW << "[ DEBUG ]" << RESET << std::endl;
	std::cout << "I love having extra bacon for my 7XL-double-cheese-triple-pickle-special-ketchup burger. I really do!" << std::endl;
}
void	Harl::info( void )
{
	std::cout << YELLOW << "[ INFO ]" << RESET << std::endl;
	std::cout << "I cannot believe adding extra bacon costs more money. You didn’t put enough bacon in my burger! If you did, I wouldn’t be asking for more!" << std::endl;
}
void	Harl::warning( void )
{
	std::cout << YELLOW << "[ WARNING ]" << RESET << std::endl;
	std::cout << "I think I deserve to have some extra bacon for free. I’ve been coming for years, whereas you started working here just last month." << std::endl;
}
void	Harl::error( void )
{
	std::cout << YELLOW << "[ ERROR ]" << RESET << std::endl;
	std::cerr << "this is unacceptable! I want to speak to the manager now" << std::endl;
}

void	Harl::complain( std::string level )
{
	std::map<std::string, void (Harl::*)()> map;

	map["DEBUG"] = &Harl::debug;
	map["INFO"] = &Harl::info;
	map["WARNING"] = &Harl::warning;
	map["ERROR"] = &Harl::error;

	try
	{
        (this->*map.at(level))();
	}
	catch (const std::out_of_range&)
	{
		std::cerr << YELLOW << "[ Probably complaining about insignificant problems ]" << RESET << std::endl;
	}
	//(this->*map[level])();
}




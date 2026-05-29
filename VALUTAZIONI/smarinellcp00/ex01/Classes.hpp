#ifndef CLASSES_H
# define CLASSES_H

# include <string>
# include <iostream>
# include <iomanip>
# include <cstdlib>

class Contact
{
	private:
		std::string	first_name;
		std::string	last_name;
		std::string	nickname;
		std::string	phonenumber;
		std::string	darkest_secret;
	public:
		Contact();
		~Contact();
		void	add();
		void	print_one_line();
		void	print_more_lines();
};

class PhoneBook
{
	private:
		Contact	contacts[8];
		int		len;
	public:
		PhoneBook();
		~PhoneBook();
		void	add();
		void	search();
};

#endif

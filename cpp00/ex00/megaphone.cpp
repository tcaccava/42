#include <iostream>
#include <string>
#include <cctype>

int	main(int c, char **v)
{
	if (!v[1]) {
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
		return 0;
	}
	for (int i = 1; i <= c - 1; i++) {
		std::string arg = std::string(v[i]);
		for(int j = 0; j < arg.length(); j++)
			std::cout << (char)std::toupper(arg[j]);
		if(i < c - 1)
			std::cout << ' ';
	}	
		std::cout << std::endl;
}
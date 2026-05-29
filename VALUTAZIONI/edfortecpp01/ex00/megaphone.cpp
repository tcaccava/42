
#include <iostream>

int main(int ac, char **av)
{
	if (ac >= 2)
	{
		int j = 0;
		while (av[++j])
		{
			unsigned long i = -1;
			std::string str = av[j];
			while (++i <= str.length())
				str[i] = toupper(str[i]);
			std::cout << str; 
		}
		return (0);
	}
	else
	{
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *\n";
		return (0);
	}
	return (1);
}
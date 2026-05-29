#include "sed.hpp"

int	main(int ac, char **av)
{
	
	check_args(ac);

	std::ostringstream	ss;
	std::string		line;
	std::string		filename = av[1];
	std::string		s1 = av[2];
	std::string		s2 = av[3];
	std::string		new_file;
	std::ifstream	infile;
	std::ofstream	outfile;
	size_t			pos;


	new_file = filename + ".replace";

	if_open_file(infile, av[1]);
	of_open_file(outfile, new_file.c_str());

	ss << infile.rdbuf();
	line = ss.str();
	
	size_t	i = 0;
	while (line[i])
	{
		pos = line.substr(i).find(s1);
		if (pos != std::string::npos)
		{
			outfile << line.substr(i, pos);
			outfile << s2;
			i += pos + s1.length();
		}
		else
		{
			outfile << line.substr(i);
			break ;
		}
	}
	
	return (EXIT_SUCCESS);
}

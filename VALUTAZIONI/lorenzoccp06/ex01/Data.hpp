#ifndef DATA_HPP
# define DATA_HPP

# include <iostream>
# include <string>

struct Data {
	int			nbr;
	std::string	str;
};

std::ostream & operator<<(std::ostream & o, Data const & i);

#endif /* ************************************************************ DATA_H */
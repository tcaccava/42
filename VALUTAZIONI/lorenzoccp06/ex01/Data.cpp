# include "Data.hpp"

std::ostream & operator<<(std::ostream & o, Data const & i) {
	o << "Data: " << i.nbr << ", " << i.str;
	return o;
}
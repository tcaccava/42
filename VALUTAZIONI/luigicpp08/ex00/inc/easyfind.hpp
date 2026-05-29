#ifndef EASYFIND_HPP
#define EASYFIND_HPP

#include <algorithm>
#include <stdexcept>

//definisco il template
template <typename T>
//definisco la funzione che lo usa. Ritornerà un iteratore
typename T::iterator easyfind(T& container, int value)
{
	//siccome sarà uno dei container sequenziali (non associativi) come vector o list
	//posso usare gli iterator che entrambi usano...e pertanto la funzione che entrambi hanno: find!
    typename T::iterator it = std::find(container.begin(), container.end(), value);


    if (it == container.end())
	{
        throw std::runtime_error("Value not found");
	}
	
	return it;	
}

#endif
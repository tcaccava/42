#ifndef SWAP_HPP
#define SWAP_HPP

#include <iostream>


template<typename Templ>
void swap(Templ &a, Templ &b)
{
    std::cout << "Swap!\n";
    Templ tmp = a;
    a = b;
    b = tmp;
}

template<typename Templ>
Templ min(const Templ &a, const Templ &b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

template<typename Templ>
Templ max(const Templ &a, const Templ &b)
{
	if (a > b)
		return (a);
	else
		return (b);
}
#endif
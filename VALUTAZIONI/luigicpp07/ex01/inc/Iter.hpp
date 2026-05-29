#ifndef ITER_HPP
# define ITER_HPP

# include<iostream>

template<typename t_arr, typename t_func>
void iter(t_arr *arr, size_t len, t_func func)
{
    size_t i;
    i = 0;
	while (i < len)
	{
		func(arr[i]);
        i++;
	}
}

template<typename t_arr, typename t_func>
void iter(const t_arr *arr, size_t len, t_func func)
{
    size_t i;
    i = 0;
	while (i < len)
	{
		func(arr[i]);
        i++;
	}
}
#endif
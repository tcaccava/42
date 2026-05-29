/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:10:12 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:10:17 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITER_H
#define ITER_H

#include <iostream>

template <typename T>

void function(T &value)
{
    std::cout << "value is: " << value << std::endl;
}
template <typename T, typename U, typename F>
void iter(T *array, const U length, F function)
{
    if(!array)
        return;
    for (U i = 0; i < length; i++)
        function(array[i]);
}

#endif

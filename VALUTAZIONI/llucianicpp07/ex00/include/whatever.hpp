/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whatever.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:10:38 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:10:41 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHATEVER_HPP
#define WHATEVER_HPP

#include <iostream>

template <typename T>
void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}
template <typename T>
T min(T &a, T &b)
{
    if(a == b)
        return b;
    return ((a < b) ? a : b);
}
template <typename T>
T max(T &a, T &b)
{
    if(a == b)
        return b;
    return ((a > b) ? a : b);
}

#endif

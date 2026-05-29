/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:09:17 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:09:17 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <stdexcept>

template<typename T>
class Array
{
    private:
            T *array;
            size_t _size;
    public:
            
            Array();
            Array(size_t n);
            Array(const Array<T>& other);
            ~Array();
            Array<T>& operator=(const Array<T>& other);
            T& operator[](size_t index);
            T& size();

            class OutofRange : public std::exception
            {
                public:
                        const char * what() const throw();
            };
};

#include "Array.tpp"

#endif
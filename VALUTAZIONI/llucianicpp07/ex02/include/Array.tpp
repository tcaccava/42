/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <lluciani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 19:09:09 by lluciani          #+#    #+#             */
/*   Updated: 2026/05/28 19:09:14 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Array.hpp"

template<typename T>
Array<T>::Array(): array(NULL), _size(0)
{
    array = new T[_size];
}

template<typename T>
Array<T>::Array(size_t n): array(NULL), _size(n)
{
    array = new T[n];
}
template<typename T>
Array<T>::~Array()
{
    delete [] array;
}

template<typename T>
Array<T>::Array(const Array<T>& other): array(NULL), _size(other._size)
{
   if(_size)
   {
        array = new T[_size];
        for(size_t i = 0; i < _size; i++)
        {
                array[i] = other.array[i];
        }
   }
}
template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    if(this != &other)
    {
        _size = other._size;
        delete[] array;
        array = new T[_size];
        for(size_t i = 0; i < _size; i++)
            array[i] = other.array[i];
    }
    return *this;
}

template<typename T>
const char* Array<T>::OutofRange::what() const throw()
{
    return "Out of the index range";
}

template<typename T>
T& Array<T>::operator[](size_t index)
{
    if (index >= _size)
        throw OutofRange();
    return array[index];
}

template<typename T>
T& Array<T>::size()
{
    return this->_size;
}

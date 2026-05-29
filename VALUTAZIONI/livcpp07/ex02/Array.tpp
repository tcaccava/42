#include "Array.hpp"

template <typename T>
Array<T>::Array(void)
{
	len = 0;
	array = new T[len];
}

template <typename T>
Array<T>::Array(unsigned int n)
{
	len = n;
	array = new T[n];
}

template <typename T>
Array<T>::Array(const Array& ref)
{
	this->len = ref.size();
	this->array = new T[len];
	for (unsigned int i = 0; i < len; i++)
		this->array[i] = ref.array[i];
}

template <typename T>
Array<T>& Array<T>::operator= (const Array& ref)
{
	delete[] this->array;
	this->len = ref.size();
	this->array = new T[len];
	for (int i = 0; i < len; i++)
		this->array[i] = ref->array[i];
	return (*this);
}

template <typename T>
unsigned int	Array<T>::size(void) const
{
	return (this->len);
}

template <typename T>
T&	Array<T>::operator [](unsigned int num)
{
	if (num > len - 1)
		throw Array<T>::outOfBondException();
	return (this->array[num]);
}

template <typename T>
Array<T>::~Array(void)
{
	delete[] array;
}

template <typename T>
const char* Array<T>::outOfBondException::what() const throw()
{
	return ("Index out of bond");
}

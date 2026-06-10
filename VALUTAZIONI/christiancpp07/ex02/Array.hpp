#ifndef ARRAY_HPP
# define ARRAY_HPP

# include <stdexcept>

template <typename T>
class Array
{
private:
	T*				_data;
	unsigned int	_size;

public:
	Array() : _data(new T[0]()), _size(0) {}

	Array(unsigned int n) : _data(new T[n]()), _size(n) {}

	Array(const Array& other) : _data(new T[other._size]()), _size(other._size)
	{
		for (unsigned int i = 0; i < _size; i++)
			_data[i] = other._data[i];
	}

	Array& operator=(const Array& other)
	{
		if (this == &other)
			return *this;
		delete[] _data;
		_size = other._size;
		_data = new T[_size]();
		for (unsigned int i = 0; i < _size; i++)
			_data[i] = other._data[i];
		return *this;
	}

	~Array() { delete[] _data; }

	T& operator[](unsigned int idx)
	{
		if (idx >= _size)
			throw std::out_of_range("Array: index out of bounds");
		return _data[idx];
	}
const T& operator[](unsigned int idx) const
	
	{
		if (idx >= _size)
			throw std::out_of_range("Array: index out of bounds");
		return _data[idx];
	}

	unsigned int size() const { return _size; }
};

#endif

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <string>
#include <iostream>

template <typename T>
class Array
{
public:
    Array() : _size(0), _array(NULL) {}
        
    Array(unsigned int n) : _size(n), _array(new T[n]()) {}

    ~Array() {delete[] _array;}

    Array(const Array &other)
    {
        this->_size = other._size;
        _array = new T[_size];
        for (int i = 0; i < _size; i++)
            _array[i] = other._array[i];
    }
    
    Array &operator=(const Array &other)
        {
            if (this != &other)
            {
                delete[] _array;
                _size = other._size;
                _array = new T[_size];
                for (int i = 0; i < _size; i++)
                    _array[i] = other._array[i];
            }
            return *this;
        }

    T &operator[](unsigned int i)
        {
            if (i >= _size)
                throw std::out_of_range("Index out of bonds");
            return _array[i];
        }

    const T& operator[]( unsigned int i) const{
        if(i >= _size)
            throw std::out_of_range("Index out of bounds");
        return _array[i];
    }
    
    unsigned int size() const{return _size;}

    private:
        T *_array;
        unsigned int _size;
    };


#endif
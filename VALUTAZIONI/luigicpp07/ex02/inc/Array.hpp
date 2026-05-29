#include <iostream>
#include <exception>
#include <cstdlib>
#include <ctime>

template<typename T>
class Array
{
    public:
    T *array;
    unsigned int len_array;

    //ctor
    Array():len_array(0)
    {
        array = NULL;
    }

    //cpy-ctor 
    Array(const Array& src) : array(NULL), len_array(0)
    {
        if (src.len_array == 0)
            return;

        array = new T[src.len_array](); 
        unsigned int i = 0;    
         
        while (i < src.len_array)
        {
            array[i] = src.array[i];
            i++;
        }
        len_array = src.len_array;
    }

    /*it is possible that src and thisObj are similar*/
    Array& operator=(const Array& src)
    {
        if (this == &src)
            return *this;

        T* tmp = NULL;
        if (src.len_array != 0)
        {
            tmp = new T[src.len_array]();    
            unsigned int i = 0;    
            while (i < src.len_array)
            {
                tmp[i] = src.array[i];
                i++;
            }
        }
        
        
         //maybe this assignment is done after the object has been 
        //created and used...and at a same point you do an assignment
        //in this case check if the object had already a memory allocated
        //for the array
        delete[] array;                           
        array = tmp;                              
        len_array = src.len_array;                

        return *this;
     
    }


    Array(unsigned int n):len_array(n)
    {
        //value inzializzation.
        //[] = array
        //T[] = array of T
        //T[n] = array of T of n elements 
        //() = inizializzazion value ... depends from type
        array = new T[len_array]();
    }

    ~Array()
    {
        delete[] this->array;
    }

    unsigned int size() const
    {
        return len_array;
    }

    //operator [] is used when you access a member of the array
    T& operator[](unsigned int i)
    {
        if (i >= len_array )
            throw Array<T>::InvalidIndexException();
        return (array[i]);
    }
    
    class InvalidIndexException: public std::exception
    {
        public:
			virtual const char	*what() const throw()
            {
                return "index out of range";
            }
    };


    const T& operator[](unsigned int i) const
    {
        
        if (i >= len_array)
        {  
            throw Array<T>::InvalidIndexException();
        }
        
        return (array[i]);
    }
};




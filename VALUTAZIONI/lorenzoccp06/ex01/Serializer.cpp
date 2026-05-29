#include "Serializer.hpp"

// https://en.cppreference.com/w/cpp/language/reinterpret_cast
// https://www.geeksforgeeks.org/reinterpret_cast-in-c-type-casting-operators/
// It is used to convert a pointer of some data type into a pointer of another data type,
// even if the data types before and after conversion are different.
// It does not check if the pointer type and data pointed by the pointer is same or not.

uintptr_t Serializer::serialize(Data* ptr) {
	return reinterpret_cast<uintptr_t>(ptr);
}

Data* Serializer::deserialize(uintptr_t raw) {
	return reinterpret_cast<Data *>(raw);
}

/* ************************************************************************** */
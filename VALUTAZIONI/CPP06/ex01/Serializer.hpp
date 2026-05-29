#pragma once
#include "Data.hpp"
#include <stdint.h>
#include <string>
#include <iostream>

class Serializer{
private:
	Serializer();
	Serializer(const Serializer &other);
	Serializer& operator=(const Serializer &other);
	~Serializer();
public:
	static uintptr_t serialize(Data* ptr);
	static Data* deserialize(uintptr_t raw);
};


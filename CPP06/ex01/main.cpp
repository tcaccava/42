#include <string>
#include <iostream>
#include <stdint.h>
#include "Serializer.hpp"
int main() {
    
    Data *a = new Data("tobia", 37, 110.5);
    std::cout << a << std::endl;
    uintptr_t serialized = Serializer::serialize(a);
    std::cout << serialized << std::endl;
    Data *b = Serializer::deserialize(serialized);
    std::cout << b << std::endl;
    if (a != b)
        std::cerr << "YOU SHOULD FAIL ME" << std::endl;
    delete a;
}
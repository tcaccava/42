#include "ScalarConverter.hpp"

int main(int c, char **v) {
    if(c == 2){
        ScalarConverter::convert(v[1]);
        return 0;
    }
    std::cerr << "Error: only one character/int/float/double argument accepted" << std::endl;
    return 1;
}
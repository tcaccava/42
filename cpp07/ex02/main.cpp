#include "Array.hpp"

int main() {
    Array<int> a(3);
    std::cout << a[2] << std::endl;
    a[2] = 4987;
    std::cout << a[2] << std::endl;
    Array<std::string> b(5);
    std::cout << b[4] << std::endl;
    b[4] = "ciaone";
    std::cout << b[4] << std::endl;

}
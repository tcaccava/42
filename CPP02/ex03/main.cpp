#include "Point.hpp"

int main()
{
    Point a(0, 0);
    Point b(5, 0);
    Point c(0, 5);

    Point inside(1, 1);
    Point outside(6, 6);
    Point on_edge(2.5f, 0);
    Point on_edge2(5,0);

    std::cout << "Point inside (1,1): "
              << (bsp(a, b, c, inside) ? "Inside" : "Outside") << std::endl;

    std::cout << "Point outside (6,6): "
              << (bsp(a, b, c, outside) ? "Inside" : "Outside") << std::endl;

    std::cout << "Point on_edge (2.5,0): "
              << (bsp(a, b, c, on_edge) ? "Inside" : "Outside") << std::endl;
    
    std::cout << "Point on_edge2 (5,0): "
              << (bsp(a, b, c, on_edge) ? "Inside" : "Outside") << std::endl;

    return 0;
}

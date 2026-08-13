#include "iter.hpp"

int main() {
    int numbers[4] = {1,2,3,4};
    std::string words[3] = {"ciaone","geeno","neeno"};
    float floating[4] = {1.2f, 6.4f, 3.13453f, 0.42f};
    char characters[4] = {'c', 'a', 'n', 'e'};
    iter(&numbers[0],4,print);
    iter(&words[0],3,print);
    iter(&floating[0], 4, print);
    iter(&characters[0],4, print);
    }

#include "Base.hpp"
#include "Functions.hpp"

int main() {
    srand(time(NULL));
    Base *casual = generate();
    Base &casual2 = *casual;
    identify(casual);
    identify(casual2);
    delete casual;
}


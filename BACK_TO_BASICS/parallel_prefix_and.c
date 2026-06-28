#include <stdio.h>
// Come parallel_prefix_or ma con AND. Propaga gli zeri verso destra.
unsigned int parallel_prefix_and(unsigned int n) {
    unsigned int origin = n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    
}
int main()
{
    printf("%#X\n", parallel_prefix_and(0xFF00FFF0));
}


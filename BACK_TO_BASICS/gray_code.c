#include <stdio.h>
// il Gray code e' un sistema di numerazione binario dove due numeri consecutivi differiscono per un solo bit
// esempio a 3 bit, 0 e' 000, 1 001, 2 011,3 010, 4 110 , 5 111, 6 101, 7 100, 8 1100
unsigned char gray_code(unsigned char n)
{
    return n ^ (n >> 1);
}
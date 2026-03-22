#include <stdio.h>

unsigned char gray_code(unsigned char n){
    return n ^ (n >> 1);
}
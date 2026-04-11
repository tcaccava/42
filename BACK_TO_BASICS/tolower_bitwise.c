#include <stdio.h>

int tolower_bitwise(int c){
    return c ^ (((c + 32) ^ c) & -(c >= 65 && c <= 90));
}

int main() {
    printf("%c %c\n", tolower_bitwise(65), tolower_bitwise(64));
}
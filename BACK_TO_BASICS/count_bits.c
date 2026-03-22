#include <stdio.h>

int count_bits(unsigned char n)
{
    int result = 0;
    for (int i = 7; i >= 0; i--)
        result += ((n >> i) & 1);
    return result;
}

// oppure algoritmo di Brian Kernighan's
//  int count_bits(unsigned char n) {
//      int result = 0;
//      while (n) {
//          n &= n - 1;  // spegne il bit più basso acceso
//          result++;
//      }
//      return result;
//  }

int main()
{
    printf("%d\n", count_bits(255)); // 8
    printf("%d\n", count_bits(128)); // 1
    printf("%d\n", count_bits(127));
}
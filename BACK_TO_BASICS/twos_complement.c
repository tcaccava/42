#include <stdio.h>
//il modo in cui il processore rappresenta i numeri negativi in aritmetica signed
int ft_twos_complement(int n) {
    return ~n + 1;  // equivale a -n
}
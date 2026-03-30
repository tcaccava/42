#include <stdio.h>
//stampa per ogni bit posizione,valore e peso(2 ^ i)
//usato in compilatori, debugger e reverse engineering. 
//È la base di strumenti come objdump, gdb, e disassembler come IDA Pro
void print_bits_detailed(unsigned char n) {
    for(int i = 7; i>= 0; i--)
        printf("bit %d: %d  peso: %d\n", i,n >> i & 1, 1 << i);
    printf("valore decimale: %d\n", n);
    printf("valore hex:      %#X\n", n);
    printf("valore ottale:   %#o\n", n);
}

int main() {
    print_bits_detailed(0b10110100);
}
#include <stdio.h>
#include "./printbits_swar.c"
// Scambia i byte adiacenti di un uint32_t: byte 0 con byte 1, byte 2 con byte 3. Usato nella conversione tra formati audio interleaved e nei protocolli di rete.
unsigned int swap_adjacent_bytes(unsigned int n) {
    unsigned int even = n & 0x55555555;
    unsigned int odds = n & 0xAAAAAAAA;
    return (even << 1) | (odds >> 1);
}

int main() {
    print_bits32(swap_adjacent_bytes(0b10111010110000001110010100011110));// 01110101110000001101101000101101
}
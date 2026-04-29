#include <stdint.h>

uint64_t popcount_swar64(uint64_t n)
{
    // 1. Sommiamo i bit a coppie (01 + 01 = 10)
    // Maschera 0x55... = 01010101...
    uint64_t sum = (n & 0x5555555555555555ULL) + ((n >> 1) & 0x5555555555555555ULL);

    // 2. Sommiamo le coppie a gruppi di 4 (nibbles)
    // Maschera 0x33... = 00110011...
    sum = (sum & 0x3333333333333333ULL) + ((sum >> 2) & 0x3333333333333333ULL);

    // 3. Sommiamo i nibbles a gruppi di 8 (bytes)
    // Maschera 0x0F... = 00001111...
    sum = (sum & 0x0F0F0F0F0F0F0F0FULL) + ((sum >> 4) & 0x0F0F0F0F0F0F0F0FULL);

    // 4. Sommiamo i byte a gruppi di 16 (short)
    sum = (sum & 0x00FF00FF00FF00FFULL) + ((sum >> 8) & 0x00FF00FF00FF00FFULL);

    // 5. Sommiamo gli short a gruppi di 32 (int)
    sum = (sum & 0x0000FFFF0000FFFFULL) + ((sum >> 16) & 0x0000FFFF0000FFFFULL);

    // 6. Sommiamo gli int a gruppi di 64 (risultato finale)
    sum = (sum & 0x00000000FFFFFFFFULL) + ((sum >> 32) & 0x00000000FFFFFFFFULL);

    return sum;
}
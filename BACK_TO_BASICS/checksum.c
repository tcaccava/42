#include <stdio.h>
#include <stdint.h>
unsigned char ft_checksum(unsigned char *data, size_t n)
{
    unsigned char checksum = 0;      // userò questo byte come accumulatore del checksum byte level
    while (n && (uintptr_t)data & 7) // allineamento ad 8
    {
        checksum ^= *data++; // lo XOR di checksum col risultato dello XOR tra checksum e data accumula nel byte il checksum
        n--;
    }
    uint64_t xored = 0; // userò questo intero a 64 bit come accumulatore del checksum SWAR level
    while (n >= 8)
    {
        uint64_t block = *(uint64_t *)data;
        xored ^= block;
        n -= 8;
        data += 8;
    }
    while (n) // bytes residui
    {
        checksum ^= *data++; // riutilizzo checksum di prima,sfruttando il principio che lo XOR e'indipendente dall'ordine degli argomenti xorati
        n--;
    }
    // arrivato qui checksum contiene lo xor dei byte iniziali e finali,e xored 8 checksumn parziali paralleli
    // devo ripiegare(folding) xored in un singolo byte,per poi fare XOR con checksum
    xored ^= xored >> 32; // ripiego da 64 a 32 bit;
    xored ^= xored >> 16; // da 32 a 16 bit
    xored ^= xored >> 8;  // da 16 a 8 bit
    return (unsigned char)xored ^ checksum;
}

int main()
{
    unsigned char data[] = {97, 32, 234};
    printf("%d\n", ft_checksum(data, 3));
}
#include <stdio.h>
#include <stdint.h>
// fa quello che fa __builtin_bswap32,cioè inverte i bytes di un uint32_t
unsigned int ft_swap_bytes(unsigned int n)
{
    uint32_t c = (uint32_t)n;
    uint32_t res = 0;
    res = (c << 24) | (c << 8 & 0x00ff0000) | (c >> 8 & 0x0000ff00) | (c >> 24);
    return (unsigned int)res;
}

int main()
{
    printf("%x\n", ft_swap_bytes(0xffabcd19));
}
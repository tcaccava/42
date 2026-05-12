#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
void print_bits32(unsigned int n)
{
    unsigned char printed0 = n & 0x000000FF;
    unsigned char printed1 = n >> 8 & 0x000000FF;
    unsigned char printed2 = n >> 16 & 0x000000FF;
    unsigned char printed3 = n >> 24 & 0x000000FF;
    uint64_t printed0_64 = printed0 * 0x0101010101010101;
    printed0_64 &= 0x8040201008040201ULL;
    printed0_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed0_64 >>= 7;
    printed0_64 &= 0x0101010101010101ULL;
    printed0_64 += 0x3030303030303030ULL;
    uint64_t printed1_64 = printed1 * 0x0101010101010101;
    printed1_64 &= 0x8040201008040201ULL;
    printed1_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed1_64 >>= 7;
    printed1_64 &= 0x0101010101010101ULL;
    printed1_64 += 0x3030303030303030ULL;
    uint64_t printed2_64 = printed2 * 0x0101010101010101;
    printed2_64 &= 0x8040201008040201ULL;
    printed2_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed2_64 >>= 7;
    printed2_64 &= 0x0101010101010101ULL;
    printed2_64 += 0x3030303030303030ULL;
    uint64_t printed3_64 = printed3 * 0x0101010101010101;
    printed3_64 &= 0x8040201008040201ULL;
    printed3_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed3_64 >>= 7;
    printed3_64 &= 0x0101010101010101ULL;
    printed3_64 += 0x3030303030303030ULL;
    printed0_64 = __builtin_bswap64(printed0_64);
    printed1_64 = __builtin_bswap64(printed1_64);
    printed2_64 = __builtin_bswap64(printed2_64);
    printed3_64 = __builtin_bswap64(printed3_64);
    write(1, &printed3_64, 8);
    write(1, &printed2_64, 8);
    write(1, &printed1_64, 8);
    write(1, &printed0_64, 8);
    write(1, "\n", 1);
}

int main()
{
    print_bits32(0xFF00FF00);
}
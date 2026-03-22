#include <emmintrin.h>  // SSE2 intrinsics
#include <stdio.h>
size_t strlen_simd(const char *s)
{
    const char *p = s;

    // scorre fino a che p non è allineato a 16 byte (opzionale)
    while (((uintptr_t)p % 16) != 0)
    {
        if (*p == 0) return p - s;
        p++;
    }

    __m128i zeros = _mm_setzero_si128();  // 16 byte a zero

    for (;; p += 16)
    {
        __m128i block = _mm_loadu_si128((const __m128i *)p);  // leggi 16 byte
        __m128i cmp = _mm_cmpeq_epi8(block, zeros);           // confronta ogni byte con 0
        int mask = _mm_movemask_epi8(cmp);                   // ottieni maschera dei bit alti

        if (mask != 0)  // almeno un byte è 0
        {
            // trova il primo byte zero
            int offset = __builtin_ctz(mask);  // conta i trailing zeros
            return (p - s) + offset;
        }
    }
}

int main() {
printf("%d\n", strlen_simd("ciao"));
}
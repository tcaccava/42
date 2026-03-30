#include <emmintrin.h>  // SSE2 intrinsics
#include <stdio.h>

size_t fast_strlen(const char *s)
{
    const char *p = s;

    while ((uintptr_t)p % 16)
    {
        if (*p == 0)
            return p - s;
        p++;
    }

    for (;;)
    {
        __m128i a = _mm_loadu_si128((__m128i*)p);
        __m128i b = _mm_loadu_si128((__m128i*)(p + 16));

        __m128i z = _mm_setzero_si128();

        int mask1 = _mm_movemask_epi8(_mm_cmpeq_epi8(a, z));
        int mask2 = _mm_movemask_epi8(_mm_cmpeq_epi8(b, z));

        if (mask1)
            return (p - s) + __builtin_ctz(mask1);

        if (mask2)
            return (p - s) + 16 + __builtin_ctz(mask2);

        p += 32;
    }
}
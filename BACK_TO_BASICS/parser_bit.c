#include <stdint.h>
#include <stdio.h>

const char *find_first_delim(const char *s) {
    const uint64_t *p = (const uint64_t *)s;

    while (1) {
        uint64_t chunk = *p;

        // crea maschere per i delimitatori
        uint64_t mask_space = ' '  * 0x0101010101010101ULL;
        uint64_t mask_comma = ','  * 0x0101010101010101ULL;
        uint64_t mask_colon = ':'  * 0x0101010101010101ULL;
        uint64_t mask_brace_open  = '{' * 0x0101010101010101ULL;
        uint64_t mask_brace_close = '}' * 0x0101010101010101ULL;

        // XOR con chunk
        uint64_t x_space  = chunk ^ mask_space;
        uint64_t x_comma  = chunk ^ mask_comma;
        uint64_t x_colon  = chunk ^ mask_colon;
        uint64_t x_bopen  = chunk ^ mask_brace_open;
        uint64_t x_bclose = chunk ^ mask_brace_close;

        // SWAR trick per trovare byte uguali
        uint64_t zeros_space  = (x_space  - 0x0101010101010101ULL) & ~x_space  & 0x8080808080808080ULL;
        uint64_t zeros_comma  = (x_comma  - 0x0101010101010101ULL) & ~x_comma  & 0x8080808080808080ULL;
        uint64_t zeros_colon  = (x_colon  - 0x0101010101010101ULL) & ~x_colon  & 0x8080808080808080ULL;
        uint64_t zeros_bopen  = (x_bopen  - 0x0101010101010101ULL) & ~x_bopen  & 0x8080808080808080ULL;
        uint64_t zeros_bclose = (x_bclose - 0x0101010101010101ULL) & ~x_bclose & 0x8080808080808080ULL;

        // OR per unire tutte le informazioni
        uint64_t any_delim = zeros_space | zeros_comma | zeros_colon | zeros_bopen | zeros_bclose;

        if (any_delim) {
            // trova il primo byte con delimitatore
            int bit_pos = __builtin_ctzll(any_delim); // posizione del bit più basso a 1
            int byte_index = bit_pos / 8;
            const char *cptr = (const char *)p;
            return cptr + byte_index;
        }

        p++; // passa al chunk successivo (8 byte)
    }
}

int main() {
    const char *s = "abc, def: {ghi}";
    const char *res = find_first_delim(s);

    if (res)
        printf("Primo delimitatore: '%c' all'indice %ld\n", *res, res - s);

    return 0;
}
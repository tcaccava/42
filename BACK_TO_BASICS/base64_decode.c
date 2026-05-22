#include <stdio.h>
#include <stdlib.h>

// Ritorna il valore Base64, -1 se invalido, -2 se padding
int b64_char_to_value(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    if (c == '=')
        return -2; // Padding distinto
    return -1;     // Invalido
}

int base64_decode(unsigned char *src, size_t src_len, unsigned char **dst)
{
    if (src_len & 3)
    {
        printf("The src is not a multiple of 4\n");
        return -2;
    }

    size_t decoded_bytes = 0;

    for (size_t i = 0; i < src_len; i += 4)
    {
        int b0 = b64_char_to_value(src[i]);
        int b1 = b64_char_to_value(src[i + 1]);
        int b2 = b64_char_to_value(src[i + 2]);
        int b3 = b64_char_to_value(src[i + 3]);

        // I primi due caratteri non possono MAI essere padding o invalidi
        if (b0 < 0 || b1 < 0)
        {
            printf("Not base64 character identified\n");
            return -1;
        }

        // Controllo coerenza padding in mezzo alla stringa
        if ((b2 == -2 || b3 == -2) && (i + 4 < src_len))
        {
            printf("Padding in the middle\n");
            return -1;
        }

        // 1° Byte: generato sempre
        **dst = (b0 << 2) | (b1 >> 4);
        *dst += 1;
        decoded_bytes++;

        // 2° Byte: generato solo se il terzo carattere non è padding
        if (b2 != -2)
        {
            if (b2 == -1)
                return -1; // Carattere invalido
            **dst = (b1 << 4) | (b2 >> 2);
            *dst += 1;
            decoded_bytes++;
        }
        else
        {
            // Se il terzo è padding, anche il quarto DEVE essere padding
            if (src[i + 3] ^ '=')
                return -1;
            break;
        }

        // 3° Byte: generato solo se il quarto carattere non è padding
        if (b3 != -2)
        {
            if (b3 == -1)
                return -1; // Carattere invalido
            **dst = (b2 << 6) | b3;
            *dst += 1;
            decoded_bytes++;
        }
        else
        {
            break;
        }
    }
    return decoded_bytes; // Ritorniamo i byte scritti per aiutare il main
}

// oppure usando una lookup table,ottenendo maggiore efficienza al costo di una leggibilita' radicalmente peggiore
//  int base64_decode_elegant(unsigned char *src, size_t src_len, unsigned char **dst)
//  {
//      // Tabella di lookup ASCII diretta (256 caratteri)
//      // -1 = invalido, -2 = padding '=', da 0 a 63 = valori Base64 reali
//      static const int b64_inv[256] = {
//          ['-1'] = -1, [0 ... 255] = -1, // Inizializza tutto a -1
//          ['A' ... 'Z'] = 0,  // Verrà riempito correttamente dal compilatore (C99 Designated Initializers)
//          ['a' ... 'z'] = 26,
//          ['0' ... '9'] = 52,
//          ['+'] = 62, ['/'] = 63, ['='] = -2
//      };

//     // Chiadiamo subito i casi limite macroscopici
//     if (src_len == 0) return 0;
//     if (src_len & 3)  return -2;

//     size_t decoded_bytes = 0;
//     unsigned int buffer = 0; // Accumulatore a 32 bit

//     for (size_t i = 0; i < src_len; i += 4)
//     {
//         // Controlliamo il padding in mezzo alla stringa (la tua intuizione ottimizzata)
//         if (i + 3 < src_len - 1 && (src[i] == '=' || src[i+1] == '=' || src[i+2] == '=' || src[i+3] == '='))
//             return -1;

//         // Carichiamo i 4 caratteri convertendoli istantaneamente
//         int v0 = b64_inv[src[i]];
//         int v1 = b64_inv[src[i+1]];
//         int v2 = b64_inv[src[i+2]];
//         int v3 = b64_inv[src[i+3]];

//         // Se uno dei primi due è padding, o se uno qualsiasi è invalido (-1), fallisce
//         if (v0 < -1 || v1 < -1 || v0 == -1 || v1 == -1 || v2 == -1 || v3 == -1)
//             return -1;

//         // Eleganza: impacchettiamo tutti i bit validi in un unico intero a 32 bit
//         // Se c'è del padding, lo trattiamo temporaneamente come 0 per non sporcare i bit
//         buffer = ((v0) << 18) |
//                  ((v1) << 12) |
//                  ((v2 == -2 ? 0 : v2) << 6) |
//                  (v3 == -2 ? 0 : v3);

//         // Scarichiamo i byte nel dst in base alla presenza del padding
//         **dst = (buffer >> 16) & 0xFF; *dst += 1; decoded_bytes++;

//         if (v2 != -2) {
//             **dst = (buffer >> 8) & 0xFF; *dst += 1; decoded_bytes++;
//         }
//         if (v3 != -2 && v2 != -2) {
//             **dst = buffer & 0xFF;        *dst += 1; decoded_bytes++;
//         }
//     }
//     return decoded_bytes;
// }
int main()
{
    unsigned char *dst = malloc(4);
    unsigned char *ptr;
    unsigned char *str;
    int res;

    // Test 1: Man
    ptr = dst;
    str = (unsigned char *)"TWFu";
    res = base64_decode(str, 4, &ptr);
    if (res >= 0)
    {
        dst[res] = '\0';
        printf("Test 1: %s\n", dst);
    }

    // Test 2: Ma
    ptr = dst; // Reset corretto del puntatore all'indirizzo base
    str = (unsigned char *)"TWE=";
    res = base64_decode(str, 4, &ptr);
    if (res >= 0)
    {
        dst[res] = '\0';
        printf("Test 2: %s\n", dst);
    }

    // Test 3: M
    ptr = dst;
    str = (unsigned char *)"TQ==";
    res = base64_decode(str, 4, &ptr);
    if (res >= 0)
    {
        dst[res] = '\0';
        printf("Test 3: %s\n", dst);
    }

    free(dst);
    return 0;
}

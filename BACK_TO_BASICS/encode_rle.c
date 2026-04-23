#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Run-Length Encoding ,comprime una sequenza di byte sostituendo sequenze ripetute con count e valore: es :AAABBC -> 3A2B1C
// E' la base di compressori come PackBits usato in TIFF e PDF

// il motivo per cui src e dst sono doppi pointer e' che in encode rle avrò necessità non solo di impostare i valori puntati,ma anche di spostare i puntatori
// e questo richiede un doppio livello di dereferenziazione
void search_bits(unsigned char **src, unsigned char **dst, size_t n, unsigned char *count, unsigned char *bytes_written, unsigned char *previous)
{
    while (n--)
    {
        if (**src == *previous && *count < 255)//check col carattere precedente e protezione dall' unsigned char overflow
            *count += 1;
        else
        {
            **dst = *count;
            (*dst)++;
            **dst = *previous;
            (*dst)++;
            *previous = **src;
            *bytes_written += 2;
            *count = 1;
        }
        (*src)++;
    }
}

unsigned char ft_encode_rle(unsigned char *src, unsigned char *dst, size_t n)
{
    unsigned char count = 1;         // count parte da 1 perchè ogni char sarà presente almeno una volta
    unsigned char bytes_written = 0; // numero di bytes scritti,da confrontare con n per valutare il grado di compressione ottenuta
    unsigned char previous = *src;   // prendo il primo carattere di src
    src++;                           // avanzo src di un carattere perche' nel ciclo while dovrò sempre confrontare il carattere successivo con previous
    n--;                             // compenso n dell'avanzamento di src
    while (n && (uintptr_t)src & 7)  // allineamento ad 8 per SWAR
    {
        if (*src == previous) // se uguale al carattere precedente incremento count
            count++;
        else // se diverso dal precedente scrivo in dst il numero count di volte in cui si e' ripetuto previous,poi aggiorno previous al carattere corrente e aggiungo 2 ai bytes scritti
        {
            *dst++ = (unsigned char)count;
            *dst++ = previous;
            previous = *src;
            bytes_written += 2;
            count = 1; // ripristino count ad 1 per rinnovare l'rle per i caratteri successivi
        }
        n--;
        src++; // nella fase di allineamento avanzo di un char alla volta
    }
    while (n >= 8) // una volta allineato procedo a gruppi di 8 bytes,SWAR puro
    {
        uint64_t block = *(uint64_t *)src;
        unsigned char c = previous;
        uint64_t mask = c * 0x0101010101010101ULL;                       // propago previous nella maschera a 64 bit                                
        if (!(block ^ mask) && count <= 247)// non c'è mismatch,posso avanzare di 8 bytes,controllo per evitare overflow di count,che e' un unsigned char
        {
            count += 8;
            src += 8;
            n -= 8;
        }
        else//almeno un bit e'diverso
        {
            search_bits(&src, &dst, 8, &count, &bytes_written, &previous); // vado in profondità nel mismatch con un approccio fallback byte by byte
            n -= 8;
        }
    }
    if (n) // approccio byte level per gli ultimi bytes rimasti
        search_bits(&src, &dst, n, &count, &bytes_written, &previous);
    *dst++ = count; // flush finale per l'ultimo char
    *dst++ = previous;
    bytes_written += 2;
    return bytes_written;
}

int main(void)
{
    unsigned char src[] = "ccccccccccccccccciiiiiiiiiaaaaaaallllllllllllllllllllllzxyxxoop";

    size_t n = strlen((char *)src);

    unsigned char dst[2 * 100]; // oppure 2 * n per sicurezza

    int written = ft_encode_rle(src, dst, n);

    printf("written = %d\n", written);

    for (int i = 0; i < written; i++)
        printf("%u ", dst[i]);

    printf("\n");

    return 0;
}
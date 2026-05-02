#include <stdio.h>
#include "./memchr_bitwise_swar.c"
#include "./memcmp_bitwise_swar.c"
#include "./memcpy_bitwise_swar.c"
#include "./count_leading_zeros.c"
// E' strstr per buffer binari,trova la prima occorrenza di needle dentro haystack.
// Sfrutto memchr e memcmp che ho già scritto in bitwise SWAR per estrema efficienza e portabilità
// Questa versione e' classificabile come un approccio naive search con accelerazione SWAR,molto veloce grazie
// al fatto che usa memchr SWAR che salta velocemente al primo byte di needle,ed ideale per needle molto corte o quando
// needle ha un primo byte raro nell'haystack,cosi che memchr SWAR filtra già quasi tutto
// unsigned char *ft_memmem(unsigned char *haystack, size_t hlen, unsigned char *needle, size_t nlen)
// {
//     unsigned char first_needle = *needle;
//     unsigned char *first_needle_in_haystack = NULL;
//     unsigned char *haystack_end = haystack + hlen;
//     if (!haystack || !needle || !hlen || nlen > hlen)
//         return NULL;
//     if (!nlen)
//         return haystack; // Un ago vuoto si trova ovunque

//     // Finché c'è abbastanza spazio tra dove sono e la fine per contenere needle
//     while (haystack <= haystack_end - nlen)
//     {
//         // Cerchiamo il primo carattere nell'area rimanente
//         first_needle_in_haystack = ft_memchr(haystack, *needle, haystack_end - haystack);

//         if (!first_needle_in_haystack) // nessuna occorrenza del primo carattere di needle,usciamo dal ciclo e ritorniamo NULL
//             break;

//         // Controllo di sicurezza: abbiamo ancora spazio per nlen byte?
//         if (first_needle_in_haystack + nlen <= haystack_end)
//         {
//             if (!ft_memcmp(first_needle_in_haystack, needle, nlen)) // abbiamo trovato un check
//                 return first_needle_in_haystack;
//         }
//         else
//             break; // Se non c'è spazio qui, non ce ne sarà più avanti

//         // Se non era lui, ripartiamo dal byte successivo a quello trovato
//         haystack = first_needle_in_haystack + 1;
//     }
//     return NULL;
// }

// Esiste una tecnica alternativa che sfrutta il così detto Vectorized Filtering, o SIMD-style double ended search,cioè cerchiamo contemporaneamente
// il primo e l'ultimo carattere di needle a gruppi di 8 bytes in parallelot disallineati di nlen. Questa versione riduce enormemente
// il numero di memcmp inutili perchè la probabilità che il primo e ultimo carattere coincidano per caso,cioè senza che poi si realizzi un match
// completo con memcmp,e'di 1/65536. Approccio consigliato per needle lunga più di 8 byte o haystack grande(MB), o se il primo byte di needle e' comune nell'haystack.
// In produzione la glibc usa esattamente una logica duplice,cioè sceglie l'algoritmo in base alla lunghezza di needle a runtime,sotto una certa soglia con un approccio
// naive,sopra con two way o vectorized/
// Esistono anche altri algoritmi,come il Boyer Moore,usato dalla glibc,o il Two Way,che sfrutta le proprità aritmetiche delle stringhe periodiche,
// ma sono estremamente difficili da implementare e sopratutto questa difficoltà non e' controbilanciata da un guadagno evidente in efficienza
// per ogni lunghezza di needle

unsigned char *ft_memmem(unsigned char *haystack, size_t hlen, unsigned char *needle, size_t nlen)
{
    if (!nlen)
        return haystack; // se needle è NULL,lo troverò in ogni haystack
    if (!haystack || !needle || !hlen || nlen > hlen)
        return NULL;
    unsigned char first_needle = *needle;             // primo carattere di needle
    unsigned char last_needle = *(needle + nlen - 1); // ultimo carattere di needle
    unsigned char *haystack_end = haystack + hlen;    // puntatore appena oltre la fine di haystack
    unsigned char *first_needle_in_haystack = NULL;
    uint64_t first_mask = first_needle * 0x0101010101010101ULL; // replico il primo carattere di needle su una maschera a 64 bit
    uint64_t last_mask = last_needle * 0x0101010101010101ULL;   // replico l'ultimo carattere di needle su una maschera a 64 bit
    size_t offset = nlen - 1;                                   // distanza tra primo e ultimo carattere di needle
    uint64_t first_chunk, last_chunk;
    // Procediamo all'approccio SWAR senza la tradizionale fase di allineamento ad 8 dei puntatori necessario per evitare il crossword
    // Allineare un puntatore ad 8,da cui poi leggeremo chunk di 8 bytes disallineati di offset,e' sostanzialmente inutile,oltre che enormemente complesso
    while (haystack + offset + 8 <= haystack_end)
    {
        ft_memcpy((unsigned char *)&first_chunk, haystack, 8);         // leggo chunk di 8 byte da haystack
        ft_memcpy((unsigned char *)&last_chunk, haystack + offset, 8); // leggo chunk di 8bytes da haystack disallineato di offset
        uint64_t first_diff = first_chunk ^ first_mask;                // XOR con entrambe le maschere
        uint64_t last_diff = last_chunk ^ last_mask;
        uint64_t first_match = (first_diff - 0x0101010101010101ULL) & ~first_diff & 0x8080808080808080ULL; // uso il bit trick che serve ad identificare la presenza di uno zero,cioè di un match in diff
        uint64_t last_match = (last_diff - 0x0101010101010101ULL) & ~last_diff & 0x8080808080808080ULL;
        uint64_t final_hits = first_match & last_match; // l'AND tra i due match mi dice se c'è un match che inizia con firstneedle e finisce con lastneedle
        if (final_hits)                                 // andiamo a controllare il check
        {
            // dal bit trick precedente,se c'è un match,cioè zero,l'unico ad uscire con il bit 7 acceso e' proprio lo zero,tutti gli altri numeri escono 0
            for (int i = 0; i < 8; i++)
            {
                if ((final_hits >> (i * 8 + 7)) & 1) // procedo byte per byte di finalhits a controllare il solo settimo bit,cioè il MSB
                {
                    if (!ft_memcmp(haystack + i, needle, nlen)) // corrispondenza trovata
                        return haystack + i;
                }
            }
        }
        haystack += 8; // avanzo il puntatore a colpi di 8 bytes,approccio SWAR puro
    }
    while (haystack <= haystack_end - nlen) // processo gli ultimi bytes rimasti dopo l'approccio SWAR
    {
        if (*haystack == first_needle)
            if (!ft_memcmp(haystack, needle, nlen))
                return haystack;
        haystack++;
    }
    return NULL;
}
int main()
{
    unsigned char haystack[] = {0, 47, 98, 102, 103, 104, 222};
    unsigned char needle[] = {102, 103, 104};
    unsigned char needle2[] = {102, 104, 103};
    printf("%u\n", *(ft_memmem(haystack, 7, needle, 3)));
    unsigned char *result = ft_memmem(haystack, 7, needle2, 3);
    if (result)
        printf("%u\n", *result);
    else
        printf("No\n");
}
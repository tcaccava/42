#include <stdio.h>
#include <stddef.h>  // per offsetof

// =============================================================================
// ALIGNMENT E ALLOCAZIONE DI MEMORIA
// =============================================================================

// -----------------------------------------------------------------------------
// COS'E' UNA WORD E COME LEGGE LA CPU
// -----------------------------------------------------------------------------
// La CPU non legge la memoria un byte alla volta — legge blocchi chiamati "word".
// Su x86-64 la word e' 8 byte (64 bit). La CPU legge sempre da indirizzi
// multipli di 8 — non puo' leggere "meta'" di una word.
//
// La memoria e' organizzata cosi':
//
// indirizzo 0:  [byte0 byte1 byte2 byte3 byte4 byte5 byte6 byte7]  ← word 0
// indirizzo 8:  [byte8 byte9 byte10 ...                  byte15]   ← word 1
// indirizzo 16: [byte16 ...                              byte23]   ← word 2
//
// Se una variabile e' contenuta interamente in una word → 1 lettura.
// Se una variabile e' a cavallo di due word → 2 letture + incollaggio.
//
// Esempio: int a 4 byte che inizia al byte 6 (NON allineato a 4):
//
// word 0: [.... .... .... .... .... .... INT0 INT1]  ← primi 2 byte dell'int
// word 1: [INT2 INT3 .... .... .... .... .... ....]  ← ultimi 2 byte dell'int
//
// La CPU deve fare 2 letture e ricombinare i pezzi — piu' lento.
//
// Stesso int che inizia al byte 4 (allineato a 4):
//
// word 0: [.... .... .... .... INT0 INT1 INT2 INT3]  ← tutto in una lettura ✓

// -----------------------------------------------------------------------------
// REQUISITI DI ALLINEAMENTO PER TIPO
// -----------------------------------------------------------------------------
// Il compilatore allinea ogni variabile alla sua dimensione naturale:
//
// char:            1 byte  → qualsiasi indirizzo
// short:           2 byte  → indirizzi multipli di 2
// int:             4 byte  → indirizzi multipli di 4
// double:          8 byte  → indirizzi multipli di 8
// SIMD __m128:    16 byte  → indirizzi multipli di 16 (SSE, AVX)
// AVX-256 __m256: 32 byte  → indirizzi multipli di 32
//
// I registri SIMD sono piu' larghi della word — la CPU li legge in operazioni
// speciali che richiedono allineamento alla loro dimensione, non alla word standard.

// -----------------------------------------------------------------------------
// x86 VS ARM — COMPORTAMENTO DIVERSO SU ACCESSO DISALLINEATO
// -----------------------------------------------------------------------------
// x86: accesso disallineato funziona ma e' piu' lento.
//      la CPU gestisce internamente le due letture e l'incollaggio.
//      penalita' tipica: 2x-3x piu' lento rispetto all'accesso allineato.
//
// ARM / MIPS / SPARC: accesso disallineato causa BUS ERROR — crash immediato.
//      il processore non gestisce il disallineamento in hardware.
//      per questo il codice portabile deve sempre rispettare l'allineamento.
//
// Conseguenza pratica: codice che funziona su x86 puo' crashare su ARM
// se usa puntatori disallineati. Comune nei protocolli di rete dove si
// castano buffer di byte a tipi strutturati senza verificare l'allineamento.

// -----------------------------------------------------------------------------
// PADDING NELLE STRUCT
// -----------------------------------------------------------------------------
// Il compilatore inserisce byte di padding tra i campi di una struct per
// garantire che ogni campo sia allineato al suo requisito.
//
// Esempio:
// struct Male {
//     char a;    // byte 0
//                // padding: byte 1, 2, 3  ← inserito dal compilatore
//     int b;     // byte 4, 5, 6, 7  (allineato a 4) ✓
//     char c;    // byte 8
//                // padding: byte 9, 10, 11
//     int d;     // byte 12, 13, 14, 15
// };
// sizeof(struct Male) = 16, non 10 come potresti aspettarti.
//
// Per minimizzare il padding, ordina i campi dal piu' grande al piu' piccolo:
// struct Bene {
//     int b;     // byte 0, 1, 2, 3
//     int d;     // byte 4, 5, 6, 7
//     char a;    // byte 8
//     char c;    // byte 9
//                // padding: byte 10, 11
// };
// sizeof(struct Bene) = 12 invece di 16.
//
// Per vedere gli offset reali dei campi usa offsetof():
// offsetof(struct Male, b) → 4  (non 1)
// offsetof(struct Male, c) → 8

// -----------------------------------------------------------------------------
// VARIABILI PIU' GRANDI DELLA WORD
// -----------------------------------------------------------------------------
// double a 8 byte che inizia al byte 0 → occupa esattamente una word. ✓
//
// array di 16 byte che inizia al byte 4 → a cavallo di tre word:
// word 0: [.... .... .... .... A0   A1   A2   A3  ]
// word 1: [A4   A5   A6   A7   A8   A9   A10  A11 ]
// word 2: [A12  A13  A14  A15  .... .... .... ....]
// tre letture — per questo i buffer SIMD richiedono allineamento a 16 o 32 byte.
//
// __m256 (AVX, 32 byte) deve iniziare a un indirizzo multiplo di 32,
// altrimenti causa penalita' di performance o crash a seconda della CPU.

// -----------------------------------------------------------------------------
// MALLOC — STRUTTURA INTERNA E LAYOUT IN MEMORIA
// -----------------------------------------------------------------------------
// malloc non alloca a botte di 16 byte — alloca esattamente quanti byte chiedi,
// ma garantisce che l'INDIRIZZO INIZIALE restituito sia multiplo di 16.
// Questo soddisfa automaticamente i requisiti di allineamento di qualsiasi
// tipo standard fino a __m128 (16 byte).
//
// LAYOUT IN MEMORIA di un blocco allocato con malloc:
//
// heap:
// [metadati 8-16 byte][     i tuoi dati     ][padding di allineamento]
//                     ↑
//                     questo e' il puntatore che malloc ti restituisce
//
// I metadati stanno PRIMA del puntatore restituito — malloc li nasconde
// all'utente. Contengono:
// - dimensione del blocco
// - flag: libero o occupato
// - puntatori al blocco precedente e successivo nella lista libera
//
// malloc sa dove sono i metadati perche' stanno sempre a offset fisso
// prima del puntatore — tipicamente ptr - 8 o ptr - 16.
// Per questo free(ptr + 1) causa undefined behavior: sposta il puntatore
// e malloc non trova piu' i metadati nel posto giusto.
//
// ESEMPIO: malloc(4) — alloco un int
//
// 0x1210-0x122F: metadati del blocco precedente (non tuoi)
// 0x1220-0x122F: metadati di questo blocco      (non tuoi, prima del ptr)
// 0x1230-0x1233: il tuo int                     (ptr punta qui)
// 0x1234-0x123F: padding                        (non tuoi, fino al prossimo multiplo di 16)
//
// ESEMPIO: malloc(4) — alloco un int — cosa vede il tuo codice:
// int *p = malloc(4);
// p == 0x1230  (multiplo di 16)
// p[0] = 42;   // scrive a 0x1230-0x1233
// free(p);     // malloc legge i metadati a 0x1220-0x122F e libera il blocco
//
// ESEMPIO: malloc(16) — alloco un __m128
// 0x1220-0x122F: metadati
// 0x1230-0x123F: il tuo __m128  (16 byte, allineato a 16) ✓
// nessun padding necessario
//
// ESEMPIO: malloc(8) — alloco un double o un long
// 0x1220-0x122F: metadati
// 0x1230-0x1237: il tuo double  (8 byte, allineato a 16, piu' che sufficiente per 8) ✓
// 0x1238-0x123F: padding
//
// OVERHEAD REALE DI MALLOC:
// malloc(1)  consuma tipicamente 16-32 byte tra metadati e padding
// malloc(4)  consuma tipicamente 16-32 byte
// malloc(16) consuma tipicamente 24-32 byte
// Per allocazioni piccole e frequenti l'overhead e' significativo —
// per questo esistono memory pool e slab allocator nel kernel.
//
// INTERNAMENTE MALLOC FA QUESTO (semplificato):
//
// void *malloc(size_t size) {
//     // cerca un blocco libero abbastanza grande nella free list
//     block_t *block = find_free_block(size + sizeof(block_t));
//     block->size = size;
//     block->free = 0;
//     void *ptr = (void*)((char*)block + sizeof(block_t));
//     // garantisce che ptr sia multiplo di 16
//     ptr = (void*)(((uintptr_t)ptr + 15) & ~15);
//     return ptr;
// }
//
// void free(void *ptr) {
//     // i metadati sono prima del puntatore
//     block_t *block = (block_t*)((char*)ptr - sizeof(block_t));
//     block->free = 1;
//     // eventualmente unisce blocchi liberi adiacenti (coalescing)
//     coalesce(block);
// }

// -----------------------------------------------------------------------------
// LA FORMULA DI ALLINEAMENTO
// -----------------------------------------------------------------------------
// Per qualsiasi k (versione generale):
// (k * ((n / k) + 1)) ^ (((k * ((n / k) + 1))) ^ n) & -(!(n % k))
// funziona per qualsiasi k, ma usa divisione e modulo — lenta.
//
// Per k potenza di 2 (versione ottimizzata usata dagli allocatori):
// (n + k - 1) & ~(k - 1)
//
// Perche' funziona:
// - k potenza di 2: k - 1 accende tutti i bit bassi (es. k=8 → k-1=00000111)
// - ~(k-1) azzera i bit bassi e accende quelli alti (es. 11111000)
// - AND con ~(k-1) tronca n al multiplo di k piu' vicino verso il basso
// - n + k - 1 spinge n abbastanza avanti da superare il multiplo corrente,
//   ma il -1 impedisce che un n gia' allineato salti al multiplo successivo
//
// Esempio: n=13, k=8
// n + k - 1 = 20 = 00010100
// ~(k-1)    = 11111000
// AND       = 00010000 = 16 ✓
//
// Esempio: n=16, k=8 (gia' allineato)
// n + k - 1 = 23 = 00010111
// ~(k-1)    = 11111000
// AND       = 00010000 = 16 ✓  (non e' saltato a 24)
//
// Perche' gli allocatori usano questa formula:
// malloc viene chiamata milioni di volte — deve essere velocissima.
// divisione: 20-40 cicli CPU
// AND + shift: 1 ciclo CPU
// Su un server con milioni di allocazioni al secondo la differenza e' reale.
//
// Uso tipico in un allocatore:
// void *ptr = current_ptr;
// current_ptr = (char*)current_ptr + align_to(size, 16);

// restituisce il prossimo multiplo di k strettamente maggiore di n
unsigned char next_multiple(unsigned char n, unsigned char k)
{
    return (k * ((n / k) + 1)) ^ (((k * ((n / k) + 1))) ^ n) & -(!(n % k));
}

// versione ottimizzata per k potenza di 2
// se n e' gia' multiplo di k (gia' allineato), lascia n invariato
unsigned char next_multiple_pow2(unsigned char n, unsigned char k)
{
    return (n + k - 1) & ~(k - 1);
}

// int main()
// {
//     // test next_multiple (versione generale)
//     printf("%d\n", next_multiple(13, 8));   // 16
//     printf("%d\n", next_multiple(16, 8));   // 24 (strettamente maggiore)
//     printf("%d\n", next_multiple(1,  4));   // 4
//     printf("%d\n", next_multiple(0,  4));   // 4
//     printf("%d\n", next_multiple(17, 8));   // 24

//     // test next_multiple_pow2 (versione ottimizzata, usata dagli allocatori)
//     printf("%d\n", next_multiple_pow2(13, 8));  // 16
//     printf("%d\n", next_multiple_pow2(16, 8));  // 16 (gia' allineato, non salta)
//     printf("%d\n", next_multiple_pow2(17, 8));  // 24

//     // padding nelle struct
//     struct Male { char a; int b; char c; int d; };
//     struct Bene { int b; int d; char a; char c; };
//     printf("sizeof Male: %zu\n", sizeof(struct Male));          // 16
//     printf("sizeof Bene: %zu\n", sizeof(struct Bene));          // 12
//     printf("offset di b in Male: %zu\n", offsetof(struct Male, b)); // 4
//     printf("offset di c in Male: %zu\n", offsetof(struct Male, c)); // 8
// }
#include <stdio.h>
#include <stdint.h>

// =============================================================================
// FT_MEMCHR — implementazione veloce con SWAR
// cerca il byte c dentro il buffer s di lunghezza n
// ritorna un puntatore alla prima occorrenza di c, o NULL se non trovato
// =============================================================================

// -----------------------------------------------------------------------------
// STRUTTURA GENERALE — tre fasi
// -----------------------------------------------------------------------------
// La CPU puo' leggere 8 byte alla volta solo da indirizzi multipli di 8.
// Se il puntatore s non e' allineato a 8, leggere un __uint64_t causa
// undefined behavior (o bus error su ARM).
// Per questo la funzione e' divisa in tre fasi:
//
// FASE 1 — allineamento: avanza byte per byte finche il puntatore
//           e' multiplo di 8. Se trovi c qui, ritorna subito.
//
// FASE 2 — SWAR: legge 8 byte alla volta e cerca c in parallelo
//           su tutti e 8 i byte simultaneamente. E' la fase veloce.
//
// FASE 3 — coda: gestisce i byte rimanenti dopo l'ultimo blocco da 8.
//           se n non e' multiplo di 8, rimangono al massimo 7 byte.

// -----------------------------------------------------------------------------
// FASE 1 — ALLINEAMENTO
// -----------------------------------------------------------------------------
// ((uintptr_t)s & 7) e' il resto della divisione dell'indirizzo per 8.
// se e' zero, il puntatore e' gia' multiplo di 8 — il while non parte.
// se e' non-zero, il puntatore e' a meta' di una word — avanziamo
// byte per byte finche non siamo allineati.
//
// Esempio:
// s = 0x1003 → 0x1003 & 7 = 3 → non allineato, avanziamo 5 byte
// s = 0x1008 → 0x1008 & 7 = 0 → allineato, partiamo con SWAR

// -----------------------------------------------------------------------------
// FASE 2 — SWAR: COME FUNZIONA LA DETECTION
// -----------------------------------------------------------------------------
// Obiettivo: trovare il byte c dentro un __uint64_t in una manciata di istruzioni,
// senza loop su 8 byte.
//
// PASSO 1: costruisci il pattern
// c * 0x0101010101010101ULL replica c in tutti e 8 i byte del __uint64_t.
// se c = 0x46 ('F'), pattern = 0x4646464646464646ULL
//
// PASSO 2: XOR block con pattern
// __uint64_t x = block ^ pattern
// XOR trasforma ogni byte uguale a c in 0x00, lascia gli altri non-zero.
// Perche': a ^ a = 0 per qualsiasi a. Quindi i byte uguali a c diventano 0,
// i byte diversi diventano qualcosa di non-zero.
// Adesso il problema e' diventato: trovare un byte zero dentro x.
//
// PASSO 3: la maschera SWAR per trovare byte zero
// __uint64_t mask = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL
//
// Questa e' la parte piu' sottile. Spieghiamo pezzo per pezzo.
//
// --- PEZZO 1: (x - 0x0101010101010101ULL) ---
// Sottraiamo 1 da ogni byte di x in parallelo.
// Cosa succede a un byte zero: 0x00 - 0x01 = 0xFF (underflow, borrow)
//   il byte diventa 0xFF e genera un borrow nel byte superiore.
// Cosa succede a un byte non-zero: 0xAB - 0x01 = 0xAA (nessun borrow)
//   il byte diminuisce di 1 normalmente.
//
// Il borrow generato dal byte zero "contagia" il byte superiore —
// per questo usiamo il pezzo 2 per isolare solo i byte che ci interessano.
//
// --- PEZZO 2: & ~x ---
// ~x inverte tutti i bit di x.
// Un byte zero in x (0x00) diventa 0xFF in ~x — bit 7 acceso.
// Un byte non-zero in x (es. 0xAB = 10101011) diventa 0x54 = 01010100 in ~x — bit 7 spento.
//
// AND tra (x - 0x0101...) e ~x:
// - per un byte zero: 0xFF & 0xFF = 0xFF — bit 7 acceso ✓
// - per un byte non-zero: qualcosa & ~(byte originale)
//   questo elimina i falsi positivi generati dal borrow del pezzo 1
//   perche' i bit accesi dal borrow non coincidono con i bit di ~x
//
// Perche' i falsi positivi?
// Se un byte vale 0x01, (0x01 - 0x01) = 0x00 — sembra un byte zero!
// Ma ~(0x01) = 0xFE = 11111110, il bit 7 e' 0.
// AND: 0x00 & 0xFE = 0x00 — falso positivo eliminato ✓
//
// --- PEZZO 3: & 0x8080808080808080ULL ---
// 0x8080... = 10000000 ripetuto 8 volte — isola solo il bit 7 di ogni byte.
// Dopo i pezzi 1 e 2 potremmo avere bit spuri accesi in posizioni 0-6.
// AND con 0x8080... li azzera tutti — teniamo solo il bit 7 di ogni byte.
// Se il bit 7 di un byte e' acceso → quel byte era zero in x → c'era c li'.
//
// RISULTATO FINALE:
// mask != 0 → almeno un byte di x era zero → c e' presente nel blocco
// mask == 0 → nessun byte di x era zero → c non e' nel blocco
//
// A questo punto iteriamo sui singoli byte per trovare la posizione esatta.

// -----------------------------------------------------------------------------
// PERCHE' NON USIAMO DIRETTAMENTE IL BIT 7 DI ~x SENZA IL PEZZO 1?
// -----------------------------------------------------------------------------
// Se usassimo solo ~x & 0x8080..., ogni byte non-zero con bit 7 = 1
// (es. 0x80, 0xFF, 0xAB) genererebbe un falso positivo.
// Il pezzo 1 serve proprio a distinguere "byte zero" da "byte con bit 7 alto".
// La combinazione (x - 0x0101...) & ~x garantisce che solo i byte zero
// producano bit 7 acceso — gli altri vengono azzerati dall'AND.

unsigned char *ft_memchr(unsigned char *s, unsigned char c, size_t n)
{

    // FASE 1: byte per byte fino all'allineamento a 8
    // ((uintptr_t)s & 7) e' il numero di byte mancanti all'allineamento
    while (n > 0 && ((uintptr_t)s & 7))
    {
        if (*s == c)
            return s;
        s++;
        n--;
    }

    // FASE 2: SWAR — 8 byte alla volta
    __uint64_t pattern = (__uint64_t)c * 0x0101010101010101ULL;
    while (n >= 8)
    {
        __uint64_t block = *(__uint64_t *)s;
        __uint64_t x = block ^ pattern;
        __uint64_t mask = (x - 0x0101010101010101ULL) & ~x & 0x8080808080808080ULL;
        if (mask)
        {
            // c trovato in questo blocco — cerca la posizione esatta
            for (int i = 0; i < 8; i++)
                if (s[i] == c)
                    return s + i;
        }
        s += 8;
        n -= 8;
    }

    // FASE 3: byte per byte per i byte rimanenti (al massimo 7)
    while (n > 0)
    {
        if (*s == c)
            return s;
        s++;
        n--;
    }

    return NULL;
}

// int main()
// {
//     unsigned char buff[] = {128, 123, 2, 11, 46, 55, 94};
//     unsigned char *result = ft_memchr(buff, 46, 7);
//     if (result)
//         printf("trovato: %d\n", *result); // 46
//     else
//         printf("non trovato\n");
// }
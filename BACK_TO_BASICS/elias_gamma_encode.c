/*
 * =====================================================================================
 * CODIFICA ELIAS-GAMMA (VARIABLE-LENGTH CODING)
 * =====================================================================================
 *
 * BASI TEORICHE:
 * La codifica Elias-Gamma è un codice universale che rappresenta interi positivi (n >= 1)
 * tramite stringhe di bit di lunghezza variabile pari a (2k + 1) bit, dove:
 * k = floor(log2(n))
 * Il codice è strutturato in due parti inscindibili:
 * 1. Prefisso Unario: k zeri seguiti da un bit '1' che funge da terminatore/separatore.
 * 2. Suffisso Binario: I k bit meno significativi di n (la componente "resto").
 *
 * Rappresentazione formale nel bitstream (LSB-to-MSB):
 * [ k zeri ] [ 1 ] [ k bit del suffisso binario ]
 *
 * APPLICAZIONI PRATICHE:
 * Questo algoritmo è la spina dorsale dei sistemi operanti in regime di "Data Compression"
 * e contesti ad alto throughput computazionale:
 * - Inverted Indexes (Lucene, Elasticsearch): Compressione dei "Delta-ID" nei
 * posting list, dove i saltelli tra documenti contigui sono numericamente piccoli.
 * - Filtri di Bloom compressi e indici bitmap ad alta densità.
 * - Codec multimediali per la trasmissione di vettori di errore residui.
 *
 * IMPLEMENTAZIONI HARDWARE:
 * Calcolare il logaritmo in base 2 tramite cicli distruggerebbe la pipeline della CPU.
 * Nello stato dell'arte moderno si mappa l'operazione direttamente sul silicio tramite
 * istruzioni dedicate della ALU in un singolo ciclo di clock:
 * - x86: BSR (Bit Scan Reverse) / LZCNT (Leading Zero Count)
 * - ARM: CLZ (Count Leading Zeros)
 * Questo codice implementa il meccanismo nativo tramite i builtins del compilatore GCC/Clang,
 * garantendo zero salti condizionali (Branchless) e performance pure.
 * =====================================================================================
 */

#include <stdio.h>
#include <stdint.h>

/**
 * Calcola floor(log2(n)) sfruttando il silicio della CPU.
 * Ritorna l'indice del bit più significativo (MSB) acceso, partendo da 0.
 */
static inline int log2_32(unsigned int n)
{
    // __builtin_clz sfrutta l'istruzione hardware della CPU (BSR/LZCNT/CLZ)
    // 31 meno i zeri iniziali restituisce l'esatta posizione dell'MSB.
    return 31 - __builtin_clz(n);
}

/**
 * Inverte l'ordine dei bit di un campo largo k bit.
 * Necessario per mantenere il suffisso binario coerente con la lettura MSB-to-LSB.
 */
static inline unsigned int reverse_bits(unsigned int val, int k)
{
    unsigned int rev = 0;
    while (k > 0)
    {
        rev = (rev << 1) | (val & 1);
        val >>= 1;
        k--;
    }
    return rev;
}

/**
 * Codifica un intero n usando l'algoritmo Elias-Gamma.
 * I bit vengono impacchettati nel registro a partire dal bit 0 (LSB-first stream).
 * Ritorna il codice Elias-Gamma compresso e pronto per il bitstream.
 */
unsigned int elias_gamma_encode(unsigned int n)
{
    // Difesa architetturale: la codifica Elias-Gamma non è definita per lo zero.
    if (n == 0)
        return 0;

    // Calcolo hardware di k = floor(log2(n))
    int k = log2_32(n);

    // Isola i k bit meno significativi (il suffisso binario)
    unsigned int suffix = n & ((1U << k) - 1);

    // Inverte i bit del suffisso per allinearli alla lettura sequenziale del flusso
    unsigned int suffix_reversed = reverse_bits(suffix, k);

    // Costruzione del codice finale (Branchless):
    // I k zeri occupano nativamente i primi k bit (da 0 a k-1).
    // Il bit '1' viene iniettato esattamente alla posizione k.
    // Il suffisso invertito viene shiftato e depositato subito dopo, a partire da k + 1.
    unsigned int encoded = (suffix_reversed << (k + 1)) | (1U << k);

    return encoded;
}

void print_bitstream(unsigned int val, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d", (val >> i) & 1);
    }
    printf("\n");
}

int main(void)
{
    // Set di test standard industriali
    unsigned int test_cases[] = {1, 2, 3, 4, 5, 6, 7, 8};

    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++)
    {
        unsigned int n = test_cases[i];
        int k = log2_32(n);
        int total_len = (2 * k) + 1; // Formula matematica per la lunghezza del codice

        unsigned int res = elias_gamma_encode(n);

        printf("Input: %2u | k = %d | Lunghezza: %2d bit | Bitstream (LSB->MSB): ", n, k, total_len);
        print_bitstream(res, total_len);
    }

    return 0;
}
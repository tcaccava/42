/**
 * @file industrial_isqrt.c
 * @brief Implementazioni industriali e analisi comparativa di algoritmi per la
 *        Radice Quadrata Intera (Integer Square Root).
 *
 * Questo file esplora tre paradigmi di calcolo a bassissimo livello, evadendo
 * l'uso della libreria matematica standard <math.h> e l'overhead del floating point.
 */

/*******************************************************************************
 * TEORIA E ANALISI DEGLI ALGORITMI DI RADICE QUADRATA INTERA A BASSO LIVELLO
 *
 * Il calcolo della radice quadrata intera senza ricorrere alla FPU (Floating
 * Point Unit) e senza conversioni di tipo e' un problema classico
 * dell'ottimizzazione a basso livello.
 *
 * -----------------------------------------------------------------------------
 * 1. ALGORITMO SWAR BRANCHLESS (Ricerca Binaria Mascherata)
 * -----------------------------------------------------------------------------
 * Questo algoritmo approccia la radice quadrata come un problema di ricerca
 * binaria sui singoli bit, costruendo il risultato dal bit piu' significativo
 * (MSB) verso il bit meno significativo (LSB).
 *
 * BASI MATEMATICHE:
 * Se un intero n ha il suo bit piu' significativo in posizione k, sappiamo
 * per definizione che:
 * 2^k <= n < 2^(k+1)
 *
 * Applicando la radice quadrata a tutti i membri, otteniamo:
 * 2^(k/2) <= radice(n) < 2^((k+1)/2)
 *
 * Questo dimostra matematicamente che l'MSB della radice quadrata si trovera'
 * sempre esattamente alla meta' della posizione dell'MSB del numero originale.
 *
 * L'algoritmo sfrutta il paradigma SWAR (SIMD Within A Register) diffondendo
 * l'MSB originale su tutti i bit inferiori per poi estrarlo. Una volta
 * posizionato a k/2, testa ogni bit scendendo verso lo zero. Se il quadrato
 * del candidato res OR bit e' <= n, il bit viene integrato nell'accumulatore.
 * L'eliminazione dei salti condizionali (Branchless) e' ottenuta moltiplicando
 * il bit per il complemento a due della valutazione booleana, che agisce come
 * un filtro bitwise perfetto (tutti 1 se vero, tutti 0 se falso).
 *
 * COMPLESSITA' COMPUTAZIONALE:
 * La complessita' temporale e' O(1) rispetto alla magnitudo dell'input. Esegue
 * sempre un numero limitato e prevedibile di iterazioni (da 1 a 16 per interi
 * a 32-bit), dipendente unicamente dalla posizione dell'MSB di partenza.
 *
 * APPLICAZIONI PRATICHE E HARDWARE:
 * - Sicurezza e Crittografia: L'assenza di salti condizionali (if/else) nel
 *   loop principale rende il tempo di esecuzione indipendente dai dati
 *   processati se si forza il loop a 16 iterazioni. Questo previene i
 *   cosiddetti Timing Attack.
 * - Architetture In-Order: Eccellente su processori senza moduli di previsione
 *   delle diramazioni, dove un branch errato costerebbe decine di cicli.
 *
 * -----------------------------------------------------------------------------
 * 2. ALGORITMO SHIFT AND SUBTRACT (Zero Moltiplicazioni)
 * -----------------------------------------------------------------------------
 * Questo metodo calcola la radice quadrata estraendo i bit in modo analogo
 * alla divisione lunga in colonna, eliminando completamente la necessita' di
 * eseguire costose moltiplicazioni hardware a 64-bit.
 *
 * BASI MATEMATICHE:
 * L'algoritmo si basa sull'identita' algebrica del quadrato di un binomio.
 * Immaginiamo che a sia la porzione di radice gia' calcolata e b sia il
 * nuovo bit da testare. Il nuovo quadrato sarebbe:
 * (a + b)^2 = a^2 + 2ab + b^2
 *
 * La differenza incrementale tra il nuovo quadrato e quello vecchio e':
 * (a + b)^2 - a^2 = 2ab + b^2 = b(2a + b)
 *
 * L'algoritmo mantiene un resto corrente sottraendo iterativamente il valore
 * b(2a + b) dal numero originario n. Poiche' in base 2 b e' sempre una
 * potenza del due, le moltiplicazioni per 2 e per b vengono tradotte dalla
 * CPU in istantanei bit-shift a sinistra (<<). Il calcolo richiede
 * esclusivamente operatori elementari: shift, addizioni e sottrazioni.
 *
 * COMPLESSITA' COMPUTAZIONALE:
 * Ha una complessita' di O(B) dove B e' l'ampiezza in bit del registro. Per un
 * intero a 32-bit, richiede esattamente 16 cicli fissi di shift e sottrazione.
 *
 * APPLICAZIONI PRATICHE E HARDWARE:
 * - Microcontrollori Bare-Metal ed Embedded: Standard industriale per CPU di
 *   fascia bassissima (es. ARM Cortex-M0,DSP cheap) che mancano fisicamente di una ALU
 *   capace di moltiplicare numeri in un singolo ciclo.
 * - FPGA e ASIC: E' l'algoritmo di riferimento per la sintesi hardware su
 *   circuiti integrati personalizzati (Verilog/VHDL), poiche' porte logiche per
 *   addizione e shift occupano una frazione microscopica di silicio rispetto a
 *   un moltiplicatore hardware.
 *
 * -----------------------------------------------------------------------------
 * 3. NEWTON-RAPHSON + ACCELERAZIONE HARDWARE (CLZ)
 * -----------------------------------------------------------------------------
 * Il metodo di convergenza numerica piu' potente, accoppiato alle istruzioni
 * specializzate presenti nel silicio delle CPU moderne per calcolare l'inversa
 * di una funzione con pochissimi cicli di clock.
 *
 * BASI MATEMATICHE:
 * Per trovare la radice x intera di n, cerchiamo lo zero della funzione
 * f(x) = x^2 - n = 0. Il metodo delle tangenti di Newton-Raphson ci fornisce
 * la formula di ricorrenza:
 * x_nuovo = (1/2) * (x_vecchio + n / x_vecchio)
 *
 * La caratteristica fondamentale di questa successione e' la sua convergenza
 * quadratica: ad ogni iterazione valida, il numero di bit corretti raddoppia.
 * Per massimizzare l'efficienza ed evitare i cicli a vuoto di un guess
 * iniziale casuale, si utilizza l'istruzione in silicio per il conteggio degli
 * zeri iniziali (Count Leading Zeros). Trovato l'MSB in 1 ciclo di clock, si
 * imposta un x_0 di partenza vicinissimo alla soluzione reale (esattamente
 * a 2^(k/2)).
 * Con un punto di partenza cosi' preciso, bastano esattamente 3 iterazioni
 * matematiche per estrarre tutti i 16 bit della radice.
 *
 * COMPLESSITA' COMPUTAZIONALE:
 * La complessita' asintotica e' O(log log n), ma nell'implementazione a 32-bit
 * collassa a O(1). Non essendoci loop ma solo 3 espressioni fisse srotolate
 * (Loop Unrolling), il costo e' puramente dato dai tempi di latenza
 * dell'istruzione di divisione hardware della CPU.
 *
 * APPLICAZIONI PRATICHE E HARDWARE:
 * - CPU Desktop e Server (x86_64, ARMv8): E' lo stato dell'arte per macchine
 *   ad alte prestazioni. Sfrutta istruzioni assembly in singolo ciclo come BSR
 *   (Bit Scan Reverse) o LZCNT.
 * - Elaborazione Vettoriale (SIMD): La totale assenza di branch e l'uso di
 *   pura matematica iterativa lo rendono il candidato ideale per il calcolo
 *   parallelo su vettori AVX2/AVX-512, dove la radice deve essere calcolata
 *   su molteplici numeri interi contemporaneamente con un'unica istruzione di
 *   blocco.
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


/* ========================================================================== */
/*  1. METODO SWAR + BRANCHLESS (Ottimizzazione Digit-by-Digit)               */
/* ========================================================================== */
/**
 * @brief Isola il bit più significativo (MSB) di un intero a 32-bit in O(1).
 *
 * Paradigma: SIMD Within A Register (SWAR).
 * Diffonde l'MSB verso destra accendendo tutti i bit sottostanti,
 * poi sottrae la metà del numero risultante per lasciare acceso solo l'MSB.
 */
static inline uint32_t keep_highest_bit(uint32_t n)
{
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n - (n >> 1);
}

static inline unsigned int popcount_swar32(unsigned int n)
{
    unsigned int even_bytes = n & 0x55555555;            // isolo i bit pari,un hex 55 e' 01010101,cioè 85
    unsigned int odd_bytes = n >> 1 & 0x55555555;        // isolo i bit dispari,prima shifto a dx,poi applico la maschera
    unsigned int sum = odd_bytes + even_bytes;           // con la somma ogni coppia di bit conterrà la somma di bit adiacenti,quindi sara 00,01 o 2,cioe' 10
    sum = (sum & 0x33333333) + (sum >> 2 & 0x33333333);  // ripeto il metodo su coppie di bit,il 33 della maschera e' 00110011,cioè 51;ora ogni nibble può valere da 0 a 4
    sum = (sum & 0x0F0F0F0F) + (sum >> 4 & 0X0F0F0F0F);  // stesso sui nibble di ogni singolo byte;ora ogni byte contiene da 0 a 8
    sum = (sum & 0X00FF00FF) + (sum >> 8 & 0x00FF00FF);  // isolo i singoli bytes,ora ogni meta' word contiene da 0 a 16
    sum = (sum & 0x0000FFFF) + (sum >> 16 & 0x0000FFFF); // isolo le coppie di bytes,ottengo il popcount
    return sum;
}

/**
 * @brief Radice quadrata intera tramite costruzione binaria (Branchless).
 *
 * APPLICAZIONE INDUSTRIALE:
 * Sistemi crittografici o engine in cui i branch (if/else) causano vulnerabilità
 * (timing attacks) o dove la CPU ha una scarsa branch prediction.
 * Esegue un numero variabile ma deterministico di iterazioni senza svuotare la pipeline.
 */
uint32_t isqrt_swar_branchless(uint32_t n)
{
    if (n < 2)
        return n; // gestisce separatamente i casi di 1 e 0,che con il resto dell'algoritmo restituirebbero entrambi 0

    uint32_t res = 0;
    uint32_t msb = keep_highest_bit(n);
    uint32_t msb_pos = popcount_swar32(msb - 1);
    /* Partiamo da metà dell'MSB, testando i bit in discesa */
    for (uint32_t bit = (1U << (msb_pos >> 1)); bit != 0; bit >>= 1)
    {
        uint32_t sqrt_cand = res | bit;

        /* Valutazione branchless: int cond vale 1 se vero, 0 se falso.
         * -cond in complemento a due diventa 0xFFFFFFFF (tutti 1) se vero, 0 se falso. */
        int cond = (uint64_t)sqrt_cand * sqrt_cand <= n;
        res |= bit & -cond;
    }
    return res;
}

/* ========================================================================== */
/*  2. METODO SHIFT AND SUBTRACT (Zero Moltiplicazioni)                       */
/* ========================================================================== */
/**
 * @brief Radice quadrata intera basata esclusivamente su ALU elementare.
 *
 * APPLICAZIONE INDUSTRIALE:
 * Microcontrollori bare-metal (es. ARM Cortex-M0), ASIC, o driver di rete scritti
 * per FPGA. Su hardware privo di modulo moltiplicatore a 64-bit, questa funzione
 * garantisce un'esecuzione velocissima (solo shift e sottrazioni).
 */
uint32_t isqrt_shift_subtract(uint32_t n)
{
    uint32_t res = 0;

    /* Il bit più alto della radice possibile per un uint32. (2^30) */
    uint32_t bit = 1U << 30;

    /* Scorre fino a trovare la potenza di 4 appena inferiore o uguale a n */
    while (bit > n)
    {
        bit >>= 2;
    }

    /* Costruisce la radice sfruttando: (a+b)^2 - a^2 = b(2a+b) */
    while (bit != 0)
    {
        if (n >= res + bit)
        {
            n -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* ========================================================================== */
/*  3. METODO NEWTON-RAPHSON + HARDWARE CLZ                                   */
/* ========================================================================== */
/**
 * @brief Radice quadrata intera tramite convergenza quadratica unrolled.
 *
 * APPLICAZIONE INDUSTRIALE:
 * Standard de facto per CPU Desktop/Server (x86_64, ARMv8).
 * Sfrutta l'istruzione in silicio BSR/LZCNT (Count Leading Zeros) per calcolare
 * il guess iniziale in 1 ciclo di clock. Raggiunge il risultato in 3 passaggi netti.
 */
uint32_t isqrt_newton_hw(uint32_t n)
{
    if (n == 0)
        return 0;

    /* 1. Trova gli zeri iniziali per ricavare l'MSB in 1 ciclo di clock hardware */
    int lz = __builtin_clz(n);
    int msb_pos = 31 - lz;

    /* 2. Stima iniziale (x0): approssimazione a 2^(msb/2) */
    uint32_t x = 1U << (msb_pos >> 1);

    /* 3. Newton-Raphson Unrolled (Loop rimosso, massimizza l'Instruction-Level Parallelism) */
    x = (x + n / x) >> 1;
    x = (x + n / x) >> 1;
    x = (x + n / x) >> 1;

   /* Gestione del troncamento: un ciclo garantisce la convergenza esatta */
    while ((uint64_t)x * x > n)
    {
        x--;
    }
    return x;
}

/* ========================================================================== */
/*  FRAMEWORK DI TEST E BENCHMARKING                                          */
/* ========================================================================== */

#define ITERATIONS 10000000

void assert_correctness()
{
    uint32_t test_cases[] = {0, 1, 2, 3, 4, 15, 16, 24, 1000000, 4294967295U}; // MAX_UINT32
    uint32_t expected[] = {0, 1, 1, 1, 2, 3, 4, 4, 1000, 65535};
    int size = sizeof(test_cases) / sizeof(test_cases[0]);

    printf("Verifica Correttezza Algoritmi...\n");
    for (int i = 0; i < size; i++)
    {
        uint32_t n = test_cases[i];
        uint32_t exp = expected[i];

        uint32_t res_swar = isqrt_swar_branchless(n);
        uint32_t res_shift = isqrt_shift_subtract(n);
        uint32_t res_newton = isqrt_newton_hw(n);

        if (res_swar != exp || res_shift != exp || res_newton != exp)
        {
            printf("[ERRORE] Test n=%u fallito!\n", n);
            printf("Atteso: %u | SWAR: %u | Shift: %u | Newton: %u\n",
                   exp, res_swar, res_shift, res_newton);
            exit(1);
        }
    }
    printf("[OK] Tutti i test passati con successo.\n\n");
}

void run_benchmark()
{
    printf("Esecuzione Benchmark (%d iterazioni)...\n", ITERATIONS);
    clock_t start, end;
    volatile uint32_t dummy = 0; // Impedisce al compilatore di rimuovere il loop (dead-code elimination)

    // Bench SWAR Branchless
    start = clock();
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        dummy += isqrt_swar_branchless(i);
    }
    end = clock();
    double time_swar = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("1. SWAR Branchless:   %f secondi\n", time_swar);

    // Bench Shift and Subtract
    dummy = 0;
    start = clock();
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        dummy += isqrt_shift_subtract(i);
    }
    end = clock();
    double time_shift = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("2. Shift & Subtract:  %f secondi\n", time_shift);

    // Bench Newton
    dummy = 0;
    start = clock();
    for (uint32_t i = 0; i < ITERATIONS; i++)
    {
        dummy += isqrt_newton_hw(i);
    }
    end = clock();
    double time_newton = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("3. Newton HW CLZ:     %f secondi\n", time_newton);
}

int main()
{
    printf("======================================\n");
    printf("   LOW-LEVEL INTEGER SQUARE ROOT      \n");
    printf("======================================\n\n");

    assert_correctness();
    run_benchmark();

    return 0;
}
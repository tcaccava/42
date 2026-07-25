#include <stdio.h>
#include "./printbits_swar.c"
/*******************************************************************************
 * TEORIA, APPLICAZIONI E IMPLEMENTAZIONI HARDWARE DELLA CODIFICA DI ZECKENDORF
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO
 * 
 * Il Teorema di Zeckendorf stabilisce che ogni intero positivo n può essere
 * scritto in modo unico come la somma di uno o più numeri di Fibonacci non
 * consecutivi. 
 * 
 * - Gestione dell'unicità e di F1/F2: Poiché i primi due numeri di Fibonacci
 *   sono entrambi uguali a 1 (F1 = 1, F2 = 1), per preservare l'unicità della
 *   rappresentazione si adotta la convenzione di escludere F1 e di far partire
 *   la sequenza utile da F2 = 1.
 * - Mappatura dei bit: Nella codifica standard, il bit i del risultato indica
 *   se il numero di Fibonacci F(i+2) fa parte della somma (es. il bit 0
 *   rappresenta F(2) = 1, il bit 1 rappresenta F(3) = 2, e così via).
 * - Algoritmo Greedy: La scomposizione sfrutta un approccio goloso (greedy)
 *   partendo dal numero di Fibonacci più grande minore o uguale a n. Questa
 *   scelta matematica garantisce automaticamente la proprietà di non-consecutività,
 *   poiché sottraendo il massimo valore disponibile il resto sarà sempre
 *   sufficientemente piccolo da impedire la selezione del numero di Fibonacci
 *   immediatamente precedente.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Compressione di Interi in Database: La codifica di Zeckendorf e le sue
 *   varianti (come i codici universali basati su Fibonacci) sono utilizzate
 *   per comprimere interi positivi, specialmente quando i valori piccoli
 *   compaiono con frequenza elevata, riducendo lo spazio di storage.
 * - Delimitazione dei Frame (Framing di Rete): Una proprietà fondamentale dei
 *   codici di Fibonacci è che la codifica binaria risultante termina sempre
 *   con due bit a 1 consecutivi (dovuti al marcatore di stop). Poiché la
 *   rappresentazione di Zeckendorf vieta tassativamente la presenza di 1
 *   consecutivi all'interno del corpo della stringa, la sequenza "11" può
 *   essere utilizzata in modo univoco e sicuro come marcatore di fine frame
 *   (End of Frame) o pattern di sincronizzazione nei protocolli di comunicazione
 *   senza bisogno di tecniche di stuffing dei dati.
 * - Strutture Dati Sparse e Indici Compatti: Ottimizzazione della memoria in
 *   indici invertiti e bitmap sparse dove la distribuzione dei dati segue
 *   andamenti non uniformi.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * - Registri a Scorrimento a Retroazione Lineare (LFSR): Le strutture ad anello
 *   e i generatori di sequenze pseudocasuali o polinomiali sfruttano
 *   correlazioni matematiche strettamente legate alla sequenza di Fibonacci e
 *   alla logica di shift e somma bitwise.
 * - Reti Combinatorie in FPGA per il Framing: Circuiti digitali dedicati
 *   all'analisi in tempo reale dei flussi di bit seriali. Utilizzano banchi
 *   di comparatori paralleli per rilevare istantaneamente i pattern di bit
 *   proibiti o i delimitatori basati su proprietà di non-consecutività,
 *   garantendo un throughput elevato a bassissima latenza senza overhead di CPU.
 * - Unità Aritmetiche per Ricerca Greedy Hardware: Pipeline hardware ottimizzate
 *   per il calcolo del massimo valore inferiore o uguale (floor/lower bound)
 *   tramite alberi di sottrazione parallela per la conversione istantanea di
 *   interi in flussi compressi on-the-chip.
 *******************************************************************************/

// unsigned int are_consecutive(unsigned int a, unsigned int b, unsigned int n)
// {
//     return b == (a << 1) - (fibonacci_gen(n - 2));
// }

unsigned int fibonacci_gen(unsigned int n)
{
    unsigned int first = 0;
    unsigned int second = 1;
    if (n == 0)
        return first;
    else if (n == 1)
        return second;
    return (fibonacci_gen(n - 1) + fibonacci_gen(n - 2));
}

unsigned int fibonacci_index(unsigned int n)
{
    if (n <= 1)
        return 2;
    unsigned int index = 3;

    while (fibonacci_gen(index) != n)
        index++;
    return index;
}

unsigned int greatest_fibonacci_lower_eq(unsigned int n)
{
    unsigned int first = 0;
    unsigned int second = 1;
    unsigned sum = 1;

    while (sum < n)
    {
        first = second;
        second = sum;
        sum = first + second;
    }
    return sum == n ? n : second;
}

void gen_zeckendorf_series(unsigned int n, unsigned int **array, unsigned int *cycles)
{
    if (n == 0)
        return;
    unsigned int num = greatest_fibonacci_lower_eq(n);
    **array = num;
    *array += 1;
    *cycles += 1;
    gen_zeckendorf_series(n - num, array, cycles);
}

unsigned int fibonacci_encode(unsigned int *array, unsigned int size)
{
    unsigned int res = 0;
    for (unsigned int i = 0; i < size; i++)
    {
        unsigned int index = fibonacci_index(array[i]);
        res |= (1U << (index - 2));
    }
    return res;
}

int main()
{
    unsigned int test[7] = {1, 2, 3, 4, 5, 11, 89};
    unsigned int cycles = 0;
    for (int i = 0; i < 7; i++)
    {
        unsigned int array[100];
        unsigned int *p = array;
        gen_zeckendorf_series(test[i], &p, &cycles);
        unsigned res = fibonacci_encode(array, cycles);
        print_bits32(res);
        cycles = 0;
    }
}
 // oppure approccio con lookup table,piu' efficiente ma meno elegante
//  unsigned int fibonacci_encode_fast(unsigned int n)
// {
//     if (n == 0)
//         return 0;

//     // Tabella statica precompilata dei Fibonacci che rientrano in uint32_t (fino a F46)
//     static const unsigned int fib[47] = {
//         0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610,
//         987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
//         121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578,
//         5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155,
//         165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903
//     };

//     unsigned int res = 0;
//     int i = 46;

//     // Trova l'indice del più grande numero di Fibonacci <= n
//     while (i >= 2 && fib[i] > n)
//     {
//         i--;
//     }

//     // Algoritmo greedy discendente
//     while (i >= 2)
//     {
//         if (n >= fib[i])
//         {
//             n -= fib[i];
//             // Mappatura diretta: F(i) corrisponde al bit (i - 2)
//             res |= (1U << (i - 2));
//             i--; // Salta il numero precedente (garantisce la non-consecutività)
//         }
//         i--;
//     }

//     return res;
// }
/* ============================================================================
 * DOCUMENTAZIONE TECNICA: GENERATORE PSEUDOCASUALE DI LEHMER (PARK-MILLER)
 * MULTIPLICATIVE LINEAR CONGRUENTIAL GENERATOR (MLCG) VIA SCHRAGE REDUCTION
 * ============================================================================
 * 1. LINEAMENTI TEORICI: L'ALGORITMO MINSTD E I PARAMETRI CRITICI
 * ----------------------------------------------------------------------------
 * Il generatore di Lehmer (noto anche come Park-Miller RNG o MINSTD) appartiene
 * alla famiglia dei generatori congruenziali lineari moltiplicativi (MLCG) in cui
 * l'incremento 'c' e' nullo. La legge di ricorrenza e' definita come:
 *
 * X_{n+1} = (a * X_n) mod m
 *
 * Nel blocco standard definito da Park e Miller, i parametri strutturali sono:
 * - Moltiplicatore (a) = 48271 (scelto per le sue eccellenti proprieta' spettrali)
 * - Modulo (m)         = 2147483647 (ovvero 2^31 - 1, un numero primo di Mersenne, M_31)
 *
 * Poiche' 'm' e' un numero primo, il generatore lavora sul gruppo moltiplicativo
 * degli interi modulo m. Scegliendo 'a' come radice primitiva di 'm', il periodo
 * massimo del generatore e' pari a m - 1 (ovvero 2.147.483.646 stati distinti).
 * Il seme iniziale (seed) puo' essere un qualsiasi intero compreso nell'intervallo
 * [1, m - 1]. Il valore 0 deve essere tassativamente evitato poiche' rappresenta
 * un punto fisso (intrappolerebbe il generatore restituendo sempre 0).
 *
 * ============================================================================
 * 2. LA RIDUZIONE DI SCHRAGE: ARITMETICA A 32 BIT SENZA OVERFLOW
 * ----------------------------------------------------------------------------
 * Il problema fondamentale risiede nel calcolo del prodotto (a * X_n).
 * Quando X_n approssima il valore massimo (m - 1), il prodotto parziale puo'
 * raggiungere:
 *
 * 48271 * (2^31 - 2) ~ 1.036 * 10^14 ~ 2^46.5
 *
 * Questo valore richiede almeno 47 bit di precisione, eccedendo ampiamente la
 * capacita' di un registro standard a 32 bit con segno, causando un overflow
 * distruttivo (Undefined Behavior in C).
 *
 * Per ovviare a questo problema senza ricorrere a tipi a 64 bit (non sempre
 * disponibili o efficienti su microcontrollori o architetture datate), si applica
 * la Riduzione di Schrage. Questa tecnica decompone il modulo 'm' sfruttando il
 * moltiplicatore 'a':
 *
 * q = m / a  (divisione intera)  => 2147483647 / 48271 = 44488
 * r = m % a  (resto della div)   => 2147483647 % 48271 = 3399
 *
 * Sfruttando questa scomposizione, la relazione di congruenza si ridisegna come:
 *
 * a * X mod m = a * (X % q) - r * (X / q)
 *
 * Proprieta' matematica di Schrage: Se r < q (condizione ampiamente soddisfatta,
 * dato che 3399 < 44488), entrambi i prodotti parziali `a * (X % q)` e 
 * `r * (X / q)` sono strettamente inferiori a 'm', garantendo che nessuna
 * operazione intermedia superi il limite superiore di un intero a 32 bit con segno.
 * Se la differenza tra i due prodotti e' negativa, e' sufficiente sommare 'm'
 * per riportare il residuo nel range corretto [1, m-1].
 * ============================================================================
 */

#include <stdio.h>

/**
 * @brief Generatore di numeri pseudocasuali di Lehmer (Park-Miller).
 * * Sfrutta la scomposizione di Schrage per l'isolamento dei registri a 32 bit
 * ed evita l'istruzione assembly MUL tramite scomposizione in shift e addizioni.
 * La correzione del segno finale e' implementata in logica branchless per 
 * azzerare i cicli di clock persi in branch misprediction.
 * * @param state Puntatore alla variabile di stato interna (seme). Range: [1, 2147483646]
 * @return unsigned int Il nuovo numero pseudocasuale generato.
 */
unsigned int ft_lehmer_rng(unsigned int *state)
{
    const unsigned int m = 2147483647; // 2^31 - 1 (Modulo di Mersenne)
    const unsigned int q = 44488;      // m / a

    // Scomposizione dello stato (quoziente e resto rispetto a q)
    unsigned int hi = *state / q;
    unsigned int lo = *state % q;

    /*-------------------------------------------------------------------------
     * MOLTIPLICAZIONE SOFTWARE VIA SHIFT-AND-ADD (Scomposizione delle costanti)
     *-------------------------------------------------------------------------
     * Scomponiamo i moltiplicatori di Schrage nella loro rappresentazione binaria:
     * * 1) a = 48271 = 0xBC8F = 1011110010001111_2
     * Bit attivi: 15, 13, 12, 11, 10, 7, 3, 2, 1, 0
     * prod_lo = lo * 48271
     * * 2) r = 3399  = 0xD47  = 110101000111_2
     * Bit attivi: 11, 10, 8, 6, 2, 1, 0
     * prod_hi = hi * 3399
     * * Nota di portabilita': Le parentesi tonde attorno ai bit-shift sono obbligatorie
     * poiche' in C l'operatore aritmetico '+' ha precedenza rispetto a '<<'.
     */
    unsigned int prod_lo = (lo << 15) + (lo << 13) + (lo << 12) + (lo << 11) + 
                           (lo << 10) + (lo << 7)  + (lo << 3)  + (lo << 2)  + 
                           (lo << 1)  + lo;

    unsigned int prod_hi = (hi << 11) + (hi << 10) + (hi << 8)  + (hi << 6)  + 
                           (hi << 2)  + (hi << 1)  + hi;

    /*-------------------------------------------------------------------------
     * LOGICA BRANCHLESS PER IL RIPRISTINO DEL SEGNO
     *-------------------------------------------------------------------------
     * Calcoliamo la differenza. Essendo entrambi i prodotti inferiori a 2^31,
     * il risultato si colloca perfettamente nel range di un int a 32 bit con segno.
     */
    int test = (int)prod_lo - (int)prod_hi;

    /*
     * Invece di usare un costoso salto condizionale (if (test < 0) test += m;),
     * estraiamo il bit di segno tramite uno shift aritmetico a destra di 31 posizioni.
     * Se test < 0, mask sara' 0xFFFFFFFF (tutti i bit a 1).
     * Se test >= 0, mask sara' 0x00000000 (tutti i bit a 0).
     * Eseguiamo un bitwise AND con la costante 'm' e sommiamo.
     */
    int mask = test >> 31;
    test += (int)(m & mask);

    *state = (unsigned int)test;
    return (*state);
}

int main(void)
{
    // Seme iniziale (deve essere diverso da 0)
    unsigned int seed = 12345;

    printf("--- SIMULAZIONE DI GENERAZIONE PSEUDOCASUALE ---\n");
    printf("Seme iniziale impostato: %u\n\n", seed);

    for (int i = 0; i < 10; i++)
    {
        unsigned int rnd = ft_lehmer_rng(&seed);
        // Normalizzazione opzionale nell'intervallo (0, 1) per calcoli statistici
        double normalized = (double)rnd / 2147483647.0;
        printf("Iterazione [%d] -> RAW: %10u | DOUBLE: %f\n", i + 1, rnd, normalized);
    }

    return (0);
}

/* ============================================================================
 * 3. OTTIMIZZAZIONE LOW-LEVEL E ANALISI MECCANICA DELLA CPU
 * ----------------------------------------------------------------------------
 * L'implementazione proposta si spinge oltre i requisiti accademici per toccare
 * la massima efficienza d'esecuzione su pipeline RISC e CISC:
 * * - Eliminazione della Hardware Multiplication:
 * Molte architetture embedded dedicate a sistemi di controllo o smart-card non
 * possiedono un moltiplicatore hardware a singolo ciclo, rendendo l'istruzione 
 * MUL dipendente da microcodice sequenziale lento (fino a 32 cicli). La traduzione
 * in shift-and-add mappa il codice direttamente su istruzioni di barrell-shifter
 * e ALU combinatorie, eseguite tipicamente in 1 solo ciclo di clock.
 * * - Filosofia Branchless e Pipeline Stall Prevention:
 * Nei moderni processori con architettura superscalare, la presenza di salti
 * condizionali attiva l'unita' di predizione dei salti (Branch Prediction Unit).
 * Trattandosi di numeri pseudocasuali, la condizione `test < 0` si verifica
 * empiricamente con una distribuzione stocastica difficile da mappare per la CPU.
 * Sostituendo il salto con l'operazione bitwise `test >> 31`, si elimina il rischio
 * di "Branch Misprediction", scongiurando lo svuotamento della pipeline (pipeline flush)
 * e risparmiando significativi cicli di clock macchina.
 * ============================================================================
 * 4. AMBITI APPLICATIVI: SIMULAZIONI MONTE CARLO E PARADIGMI DI GIOCO
 * ----------------------------------------------------------------------------
 * Il generatore di Lehmer Park-Miller non e' crittograficamente sicuro (CSPRNG),
 * poiche' soffre della tipica linearita' geometrica intrinseca dei metodi congruenziali.
 * Tuttavia, rappresenta lo standard industriale de facto in svariati scenari:
 * * - Simulazioni Monte Carlo:
 * Nello studio dei flussi di particelle, simulazioni economiche, o modellizzazione
 * fisica, la velocita' computazionale e' la risorsa piu' critica. L'algoritmo di Lehmer
 * offre un bilanciamento perfetto tra uniformita' della distribuzione statistica
 * e throughput di generazione.
 * * - Game Development e Logica Deterministica (Procedural Generation):
 * Nei videogiochi, la riproducibilita' della generazione procedurale (es. la creazione
 * di mappe o la traiettoria di particelle) dipende interamente dal seme.
 * Forzando lo stesso seed, il generatore replichera' fedelmente la medesima sequenza
 * di eventi su qualsiasi piattaforma, facilitando il netcode predittivo (Client-Side
 * Prediction) e la sincronizzazione multiplayer peer-to-peer basata solo sullo scambio
 * dello stato iniziale.
 * ============================================================================
 */
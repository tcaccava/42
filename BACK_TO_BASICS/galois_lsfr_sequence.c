/*
 * =====================================================================================
 * GALOIS_LFSR - GENERATORE DI SEQUENZE PSEUDO-CASUALI (PARALLEL FEEDBACK)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * Il registro LFSR (Linear Feedback Shift Register) di Galois è l'architettura
 * di riferimento per la generazione di sequenze a lunghezza massima (MLS) in ambito
 * hardware. A differenza della variante di Fibonacci, dove il feedback viene
 * accumulato in una catena seriale di XOR prima di essere re-iniettato, il modello
 * di Galois applica il feedback in modo distribuito e parallelo.
 * * LOGICA OPERATIVA:
 * In Galois, l'intero registro agisce come un automa che trasforma il proprio stato
 * in un singolo ciclo di clock. Quando il bit di uscita (bit 0) è 1, il polinomio
 * di feedback viene applicato istantaneamente tramite XOR all'intero registro,
 * evitando la propagazione seriale che limita la frequenza operativa in Fibonacci.
 *
 * FIBONACCI VS GALOIS:
 * Entrambi generano sequenze pseudocasuali(PRNG) massimamente lunghe(MLS), ma operano
 * in modo profondamente diverso.
 * - FIBONACCI (Modello Seriale): Il registro e' trattato come un unico blocco monolitico:
 * il bit di output che esce da un'estremita' viene passato attraverso una catena di porte XOR
 * con i tap(i bit definiti dal polinomio) e il risultato reiniettato esclusivamente nel bit piu'
 * significativo(il bit di input del registro).Il feedback è "additivo" e centralizzato: poiche' il fedback
 * deve attraversare una lunga catena di porte XOR prima di raggiungere l'input,si crea un collo di bottiglia nella
 * propagazione del segnale. Il ritardo critico è proporzionale al numero di tap (N-gate XOR in serie). Cio'
 * limita il clock massimo,perche' il segnale deve viaggiare lungo tutta la catena prima di aggiornare il registro.
 * Usato per lo piu' in implementazioni software veloci dove il tempo di calcolo e' irrilevante.
 * - GALOIS (Modello Parallelo): Il feedback è "trasformativo" e distribuito,cioe' locale.Quando il bit di output viene generato
 * ,non viene solo spostato nel registro,ma inviato in parallelo a tutte le posizioni definite dai tap del polinomio,e ognuna
 * di quelle posizioni esegue uno XOR tra il suo valore corrente e il bit di feedback in un unico istante.
 * Non c'e' una catena seriale di XOR,ogni bit del registro e' un unita' indipendente che riceve il feedback e aggiorna il suo
 * stato autonomamente.Il ritardo critico è ridotto alla propagazione elettrica attraverso una singola porta logica (XOR), indipendentemente dal polinomio.
 * operando cosi' a frequenze estremamente piu' alte. È l'architettura scelta per ASIC, FPGA e sistemi di telecomunicazione ad alta velocità.
 * Quando devi gestire flussi di dati a Gigahertz (es. nel DVB-T o nelle comunicazioni satellitari), non puoi permetterti il ritardo di
 * propagazione della struttura di Fibonacci. La struttura di Galois è intrinsecamente "nativa" per il silicio: garantisce che il registro possa
 * aggiornarsi nel tempo minimo necessario per una singola operazione booleana, permettendo al hardware di girare
 * alla massima velocità possibile.
 *
 * IMPLEMENTAZIONI INDUSTRIALI:
 * - ASIC/FPGA: Il modello di Galois è lo standard per la generazione di PRBS (Pseudo-Random
 * Binary Sequence) per il test di integrità del segnale. Non esiste "calcolo",
 * esiste solo "propagazione elettrica".
 * - Crittografia Hardware: Usato come base per stream ciphers leggeri (es. A5/1, E0)
 * dove la velocità di generazione del keystream è critica per minimizzare il consumo.
 * - Digital Signal Processing: Fondamentale per la sincronizzazione spread-spectrum
 * (es. CDMA, GPS) dove il correlatore deve rigenerare la sequenza di chip in tempo reale.
 * =====================================================================================
 */

#include <stdio.h>
#include <stddef.h>

/**
 * @brief Genera una sequenza pseudo-casuale utilizzando un LFSR di Galois a 16-bit.
 * @param seed Stato iniziale del registro (deve essere diverso da 0).
 * @param poly Polinomio di Galois (maschera dei tap allineata).
 * Nota: Il polinomio deve essere convertito nella forma adatta a Galois.
 * @param out Puntatore al buffer dove memorizzare la sequenza generata.
 * @param n Numero di iterazioni da eseguire.
 */
void galois_lfsr_sequence(unsigned short seed, unsigned short poly, unsigned short *out, size_t n)
{
    unsigned short state = seed;

    for (size_t i = 0; i < n; i++)
    {
        /* 1. ESTRAZIONE BIT DI USCITA:
         * In un registro che shifta a destra, il bit che esce è sempre il bit 0.
         */
        unsigned short exit_bit = state & 1;

        /* 2. SHIFT E FEEDBACK:
         * Se il bit di uscita è 1, applichiamo lo XOR con il polinomio di Galois.
         * Questa operazione è atomica ed eseguita in parallelo sull'intero registro.
         */
        state >>= 1;
        if (exit_bit)
            state ^= poly;

        /* 3. STORAGE:
         * Memorizziamo lo stato raggiunto per analisi statistiche o cifratura.
         */
        out[i] = state;
    }
}

int main(void)
{
    // Esempio: Registro 16-bit, Polinomio 0xB400 (forma di Galois)
    unsigned short out[10];
    galois_lfsr_sequence(0xACE1, 0xB400, out, 10);

    for (int i = 0; i < 10; i++)
        printf("Iterazione %d: 0x%04X\n", i, out[i]);

    return 0;
}
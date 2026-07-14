#include <stdio.h>
#include "./popcount_swar32.c"
/**
 * =====================================================================================
 * @file        bit_balance.c
 * @brief       Calcolo della disparità binaria (DC-balance) tramite ottimizzazione SWAR.
 *
 * 1. DEEP-DIVE TEORICO: OMOMORFISMO TRA IPERCUBI E SPAZI DI SPIN
 *    
 *    A) Mappatura dallo spazio Booleano allo spazio dei Segni
 *       L'algoritmo esegue una trasformazione affine per passare dallo spazio booleano 
 *       {0, 1}^32 allo spazio discreto dei segni {-1, +1}^32. 
 *       Sia S il numero di bit impostati a 1 (Hamming Weight o Popcount) e U il numero 
 *       di bit impostati a 0. Per definizione di un registro a N bit (qui N = 32):
 *           S + U = 32  -->  U = 32 - S
 *       La bilancia dei bit (B) è definita come la differenza quantitativa B = S - U.
 *       Sostituendo U nell'equazione otteniamo:
 *           B = S - (32 - S)  -->  B = 2S - 32
 *       L'espressione `(popcount(n) << 1) - 32` implementa esattamente questa equazione, 
 *       sfruttando lo shift logico a sinistra di 1 posizione come primitiva hardware 
 *       per la moltiplicazione per 2.
 *
 *    B) Connessione con la Trasformata di Walsh-Hadamard e Modelli di Spin
 *       In teoria dell'informazione, questa operazione equivale a calcolare la componente 
 *       DC (corrente continua) di un vettore di bit. È il nucleo del calcolo della 
 *       correlazione nei sistemi che usano variabili di Rademacher o modelli di spin 
 *       in fisica statistica (Ising Model), dove lo stato 0 logico mappa a -1 fisico 
 *       e lo stato 1 logico mappa a +1 fisico.
 *
 * 2. APPLICAZIONI PRATICHE DI LIVELLO INDUSTRIALE:
 *    
 *    A) Binary Neural Networks (BNN) e 1-bit LLM
 *       Nelle reti neurali quantizzate a 1 bit, i pesi e le attivazioni non sono float 
 *       ma singoli bit (0 rappresenta -1, 1 rappresenta +1). Il prodotto scalare tra 
 *       due vettori di quantizzati viene calcolato eseguendo un XNOR bitwise tra i due 
 *       vettori (che restituisce 1 dove i segni concordano) e calcolando il bit_balance 
 *       del risultato. Questo abbatte il costo computazionale dei layer lineari a zero.
 *    
 *    B) Line Coding e DC-Balance nei Protocolli di Rete
 *       Nelle trasmissioni seriali ad alta velocità (es. PCIe, SATA, Ethernet 10G/64b/66b), 
 *       è fondamentale che il flusso di bit sia "DC-Balanced" (stesso numero di 1 e 0) 
 *       per evitare la saturazione capacitiva dei canali e mantenere il sincronismo del clock. 
 *       Questa funzione viene usata per monitorare la disparità del flusso in tempo reale.
 *
 * 3. ANALISI MICROARCHITETTURALE E PIPELINE:
 *    
 *    A) Esecuzione Branchless e Throughput
 *       L'algoritmo non contiene salti condizionali. L'assenza totale di costrutti `if` 
 *       garantisce che l'unita di predizione dei salti della CPU (Branch Predictor) non 
 *       possa fallire, eliminando il rischio di stalli della pipeline (pipeline flushes).
 *    
 *    B) Ottimizzazione delle Istruzioni Assembly
 *       Se il target hardware supporta l'estensione SSE4.2/AVX (istruzione nativa POPCNT), 
 *       il compilatore traduce l'intera funzione in tre sole istruzioni macchina:
 *           popcnt eax, edi     ; Calcola S in 1 ciclo di clock
 *           add    eax, eax     ; Moltiplica per 2 (equivalente a << 1, ottimizzato dalla ALU)
 *           sub    eax, 32      ; Sottrae 32 per ottenere il bilancio finale
 *       Questa sequenza ha una latenza complessiva di circa 4 cicli di clock e un 
 *       throughput di 1 ciclo su core x86 moderni.
 *    
 *    C) Ottimizzazione SWAR (Fallback)
 *       Se compilato senza istruzioni native, l'inclusione di `popcount_swar32` esegue 
 *       il conteggio in parallelo strutturato tramite maschere bitwise (riduzione logaritmica 
 *       in albero). Anche in modalità fallback, l'intera routine viene eseguita interamente 
 *       nei registri generici (GPR), evitando accessi alla memoria cache L1.
 * =====================================================================================
 */

#include <stdio.h>
#include "./popcount_swar32.c"

// Ritorna la differenza tra il numero di bit accesi e il numero di bit spenti in n. 
// Esempio su 8 bit: n=0b10110100 ha 4 bit accesi e 4 spenti -> risultato 0.
// Su contesti a 32 bit, i bit non specificati a sinistra sono considerati 0.
int bit_balance(unsigned int n)  {
    return (popcount_swar32(n) << 1) - 32;
}

int main() {
    // 0xFF0F0F0F = 11111111 00001111 00001111 00001111
    // Bit accesi (1): 8 + 4 + 4 + 4 = 24
    // Bit spenti (0): 0 + 4 + 4 + 4 = 8
    // Bilancio atteso: 24 - 8 = 16
    printf("%d\n", bit_balance(0xFF0F0F0F));
    return 0;
}
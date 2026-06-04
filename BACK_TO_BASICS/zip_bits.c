#include <stdio.h>

// =============================================================================
// BACKGROUND TEORICO
// =============================================================================
// Questa funzione implementa un "Bitwise Multiplexer" (o selettore bit a bit).
// L'obiettivo è fondere due registri numerici applicando una condizione logica
// parallela su ogni singolo bit, escludendo qualsiasi costrutto condizionale (if/else).
//
// Il meccanismo si basa sulla scomposizione dello spazio booleano in due canali:
// 1. Il Canale A (~mask & a): L'operatore NOT inverte la maschera. I bit che
//    erano 0 diventano 1. L'AND logico successivo agisce come un filtro passa-alto:
//    mantiene inalterati i bit di a solo nelle posizioni in cui la maschera originale
//    era zero, azzerando tutto il resto.
// 2. Il Canale B (mask & b): L'AND logico diretto tra la maschera e b lascia
//    passare i bit di b esclusivamente dove la maschera presenta un bit acceso (1).
//
// La fusione finale avviene tramite l'operatore OR (|). Poiché i canali A e B sono
// per definizione mutuamente esclusivi (dove uno ha bit attivi, l'altro ha zeri),
// l'OR combina le due metà specchiate senza collisioni o interferenze aritmetiche,
// realizzando uno switch logico parallelo.
//
// =============================================================================
// APPLICAZIONI PRATICHE
// =============================================================================
// Il bitwise multiplexing è un pilastro nello sviluppo di sistemi a basso livello:
//
// - Funzioni Crittografiche (SHA-256 / MD5): Questa esatta logica è la base della
//   funzione primitiva "Ch" (Choose) di SHA-256, definita formalmente come
//   Ch(x, y, z) = (x & y) ^ (~x & z). Sostituendo lo XOR con l'OR (equivalenti
//   in questo scenario di disgiunzione), si ottiene la stessa identica struttura.
// - Bit-Blitting e Grafica 2D Software: Storicamente utilizzato nei motori grafici
//   per applicare maschere di trasparenza (sprite masking). Permette di disegnare
//   un pixel sul display solo se la maschera associata permette il passaggio del
//   colore dello sprite, lasciando intatto lo sfondo in caso contrario.
// - Modifica di Registri Hardware (Bit-Field Update): Nei driver di periferica,
//   quando è necessario aggiornare un gruppo di bit centrali dentro un registro
//   di controllo hardware senza alterare i bit di configurazione circostanti.
//
// =============================================================================
// IMPLEMENTAZIONE HARDWARE & ARCHITETTURA ISA (x86-64 / SIMD)
// =============================================================================
// A livello di silicio, l'ottimizzazione di questa operazione è così critica che
// l'evoluzione dell'architettura hardware dei processori l'ha integrata nativamente
// a tre livelli sequenziali di complessità geometrica:
//
// 1. CIRCUITO COMBINATORIO DI BASE (Gate Level):
//    Per ogni linea di bit del registro a 32 bit, il circuito reale sul silicio è
//    composto da 1 porta NOT, 2 porte AND e 1 porta OR eseguite in parallelo puro.
//    Non essendoci istruzioni di salto hardware (Branch), la pipeline della CPU
//    non subisce mai penalità da macro-errori di predizione del flusso (Branch Misprediction).
//    Il tempo di esecuzione è deterministico e costante.
//
// 2. LIVELLO SCALARE CPU MODERNE (Estensioni BMI1 - Istruzione ANDN):
//    Nel C standard, l'espressione (~mask & a) richiede due istruzioni assembly discrete:
//    un NOT per invertire la maschera e un AND. Dal 2013, con l'introduzione dell'ISA BMI1
//    (Bit Manipulation Instructions 1), Intel/AMD gestiscono questa operazione tramite
//    l'istruzione nativa hardware 'ANDN' (AND Not). Questa istruzione inverte il primo
//    operando ed esegue l'AND con il secondo in un unico fottuto ciclo di clock,
//    eliminando fisicamente lo stadio di NOT dall'esecuzione hardware.
//
// 3. LIVELLO VETTORIALE SIMD (Estensioni SSE4.1 / AVX2 - Istruzione PBLENDVB):
//    Quando l'operazione deve essere parallelizzata su array massivi di dati, i compilatori
//    utilizzano i registri vettoriali da 128 o 256 bit. Con il set SSE4.1 viene introdotta
//    l'istruzione PBLENDVB (Packed Blend Variable Byte). L'hardware legge il bit più
//    significativo (MSB) di ogni byte del registro maschera: se è 1 seleziona il rispettivo
//    byte dal vettore B, se è 0 dal vettore A. Questo azzera i cicli di clock nell'elaborazione video.
//
// 4. L'APICE ARCHITETTURALE (AVX-512 - Istruzione VPTERNLOGD):
//    Nelle estensioni AVX-512, i progettisti hanno rimpiazzato le singole porte logiche discrete
//    con un'unità hardware a logica ternaria programmabile. L'istruzione 'VPTERNLOGD' accetta
//    tre registri a 512 bit e un valore "immediato" a 8 bit che funge da Tabella della Verità.
//    Per calcolare l'intera espressione di zip_bits, ovvero f(A,B,Mask) = (A & ~Mask) | (B & Mask),
//    il compilatore passa l'immediato fisso '0xCA'. L'hardware della CPU riconosce la configurazione
//    della tabella booleana associata a 0xCA, mappa istantaneamente le porte interne e restituisce
//    il multiplexing di 512 bit di dati contemporaneamente in un solo ciclo di clock.
// =============================================================================

unsigned int zip_bits(unsigned int a, unsigned int b, unsigned int mask)
{
    return (~mask & a) | (mask & b);
}

int main()
{
    // Input di test:
    // a    = 0x00000063  ->  0110 0011
    // b    = 0x000000DD  ->  1101 1101
    // mask = 0x00000013  ->  0001 0011
    //
    // Risultato atteso: 0x71 -> 0111 0001
    printf("%#x\n", zip_bits(0x00000063, 0x000000DD, 0x00000013)); // 0x71
    return 0;
}
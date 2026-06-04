#include <stdio.h>

// =============================================================================
// BACKGROUND TEORICO
// =============================================================================
// Questa funzione calcola la "funzione di maggioranza" (Majority Function) bit a bit.
// Dati tre registri di ingresso (a, b, c), l'output presenterà un bit a 1 in una 
// specifica posizione se e solo se almeno due dei tre ingressi presentano un 1
// nella medesima posizione.
//
// Dal punto di vista dell'algebra booleana, la funzione è espressa come:
// f(a, b, c) = (a & b) | (b & c) | (a & c)
//
// L'inclusione esplicita del caso in cui tutti e tre i bit 
// siano accesi, ovvero (a & b & c), è logicamente superflua a causa della 
// Legge dell'Assorbimento dell'algebra di Boole:
// A | (A & B) = A
// Se tutti e tre gli ingressi sono pari a 1, ogni singola intersezione a coppie 
// (a & b), (b & c), (a & c) valuterà 1. Di conseguenza, l'OR finale produrrà 1 
// in modo nativo, assorbendo completamente l'intersezione ternaria senza la necessità 
// di computarla isolatamente.
//
// =============================================================================
// APPLICAZIONI PRATICHE
// =============================================================================
// Il calcolo della maggioranza bitwise trova applicazione in due domini critici:
//
// 1. TRIPLA RIDONDANZA MODULARE (TMR - Triple Modular Redundancy):
//    Utilizzata in sistemi avionici, satellitari e mission-critical per garantire 
//    la tolleranza ai guasti (Fault-Tolerance). I raggi cosmici o le radiazioni 
//    ionizzanti nello spazio profondo causano alterazioni fisiche dello stato logico 
//    dei semiconduttori, chiamate SEU (Single Event Upset) o bit-flip. 
//    Eseguendo lo stesso calcolo su tre chip hardware specchiati (a, b, c), questa 
//    funzione funge da "votatore software" immediato, scartando il canale corrotto 
//    e preservando l'integrità del dato in tempo reale.
//
// 2. ALGORITMI CRITTOGRAFICI (Primitiva Maj in SHA-256):
//    Insieme al bitwise multiplexer (funzione Choose), la funzione Majority è la 
//    seconda primitiva non lineare fondamentale applicata nei 64 round core dello 
//    standard SHA-256. Serve a distruggere la linearità matematica dei bit del 
//    messaggio espanso, impedendo attacchi basati su crittoanalisi differenziale.
//
// =============================================================================
// IMPLEMENTAZIONE HARDWARE & ARCHITETTURA ISA
// =============================================================================
// La struttura simmetrica della funzione di maggioranza permette una mappatura 
// hardware ad altissima efficienza energetica e prestazionale:
//
// 1. SINTESI A LIVELLO DI PORTA (Mappe di Karnaugh):
//    La minimizzazione logica tramite mappa di Karnaugh dimostra che il circuito 
//    ottimale per cella richiede esattamente 3 porte AND a due ingressi collegate 
//    a una singola porta OR a tre ingressi. Riducendo al minimo il conteggio dei 
//    transistor e i cammini critici, si minimizza il ritardo di propagazione (Gate Delay) 
//    e si riduce la superficie di silicio esposta a potenziali impatti da radiazioni.
//
// 2. DETERMINISMO BRANCHLESS:
//    L'assenza di costrutti condizionali (if/else) elimina la necessità di interrogare 
//    le unità di predizione dei salti (Branch Prediction Units) della CPU. Il tempo 
//    di calcolo è costante e rigidamente deterministico, una specifica tassativa per 
//    i sistemi operativi Real-Time (RTOS) impiegati nel controllo di volo.
//
// 3. VETTORIZZAZIONE AVX-512 (Istruzione VPTERNLOGD):
//    Nelle architetture moderne x86 che supportano i registri vettoriali da 512 bit 
//    (ZMM), questa intera operazione logica a tre variabili non viene eseguita tramite 
//    istruzioni discrete di AND e OR. Viene invocata la singola istruzione hardware 
//    VPTERNLOGD (Vector Packed Ternary Logic). Passando il valore immediato a 8 bit 
//    0xE8 (che rappresenta la tabella della verità esatta della funzione Majority), 
//    la CPU configura la sua rete logica interna programmabile ed esegue il voto 
//    di maggioranza su 16 interi a 32 bit contemporaneamente in un singolo ciclo di clock.
// =============================================================================



unsigned int bit_majority(unsigned int a, unsigned int b, unsigned int c) {
    return (a & b) | (b & c) | (a & c);
}

int main() {
    // Input di test:
    // a = 15 -> 0000 1111
    // b = 11 -> 0000 1011
    // c = 39 -> 0010 0111
    // 
    // Analisi colonne (da destra a sinistra):
    // Bit 0: a=1, b=1, c=1 -> Maggioranza = 1
    // Bit 1: a=1, b=1, c=1 -> Maggioranza = 1
    // Bit 2: a=1, b=0, c=1 -> Maggioranza = 1
    // Bit 3: a=1, b=1, c=0 -> Maggioranza = 1
    // Bit 4: a=0, b=0, c=0 -> Maggioranza = 0
    // Bit 5: a=0, b=0, c=1 -> Maggioranza = 0
    //
    // Risultato atteso: 15 -> 0000 1111
    printf("%u\n", bit_majority(15, 11, 39));
    return 0;
}
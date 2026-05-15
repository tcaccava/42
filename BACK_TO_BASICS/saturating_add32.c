#include <stdio.h>

// Utilizzi pratici dell'Addizione Saturata (Saturating Add)

// L'addizione saturata è un'operazione fondamentale in tutti gli ambiti in cui l'**overflow ciclico** (wrap-around) dei registri numerici
// causerebbe anomalie catastrofiche, corruzione dei dati o comportamenti instabili del sistema. A differenza dell'addizione standard,
// se il risultato supera il valore massimo rappresentabile, viene forzato (saturato) al limite estremo del tipo di dato (es. `UINT_MAX`).

// 1)Elaborazione dei segnali digitali (DSP) e audio
// Nel processing audio in tempo reale, la somma di due segnali (ad esempio nel missaggio di due tracce) può facilmente superare la dinamica massima
// consentita. Senza saturazione (Wrap-around),se il valore di un picco audio supera il massimo, si azzera bruscamente, invertendo la polarità dell'onda.
// Questo genera un rumore digitale ad alta frequenza distruttivo per gli altoparlanti e i timpani.
// Con la saturazione il segnale viene bloccato al livello massimo (clipping): pur introducendo una distorsione armonica, preserva la forma d'onda originale,
//  imitando il comportamento dei circuiti analogici e salvaguardando l'hardware.

// 2)Computer vision ed elaborazione digitale delle immagini
// Nelle operazioni bitwise sui pixel (regolazione della luminosità, fusione di livelli o calcolo dei contrasti), i canali colore sono spesso rappresentati
// a 8 bit (0-255 per R, G, B). Se si aumenta la luminosità di un pixel con valore 250 di un fattore 10,senza saturazione il pixel assume il valore 4, diventando
// improvvisamente nero anziché bianco puro. Il risultato visivo è un artefatto grafico disastroso.
// Con saturazione il valore viene inchiodato a 255, garantendo che le aree sovraesposte rimangano correttamente bianche.

// 3)Sistemi embedded e contatori critici (automotive/aerospace)
// Nei sistemi operativi real-time (RTOS) e nei microcontrollori che gestiscono sensori di sicurezza (es. freni ABS, controllo trazione), i contatori di eventi
// o i tempi di timeout non devono mai azzerarsi da soli. Se un contatore di errori hardware andasse in overflow tornando a zero, il sistema registrerebbe
// una situazione di "nessun errore", ignorando un guasto potenzialmente letale. La saturazione garantisce che lo stato di allarme massimo persista fino a un reset esplicito.

// 4)Game development (sistemi di punteggio e statistiche)
// Nei motori di gioco, l'addizione saturata viene usata per gestire i punti ferita (HP), l'esperienza o i contatori di valuta in-game. Evita che un accumulo
// eccessivo di punti si traduca istantaneamente nella morte del personaggio o nell'azzeramento dell'inventario a causa del rollover delle variabili.
unsigned int saturating_add32(unsigned int a, unsigned int b)
{
    unsigned int sum = a + b;
    return sum | -(sum < a);
}

int main()
{
    printf("%#X %#X\n", saturating_add32(0xFFFFFFFF, 0x00FF00FF), saturating_add32(0x00000010, 0x0000033));
}
#include <stdio.h>

/* ========================================================================== */
/* BIT_FIELD_INSERT.C - INSERIMENTO DI CAMPI DI BIT SENZA EFFETTI COLLATERALI*/
/*
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/* 1. BACKGROUND TEORICO: IL PARADOSSO DELLA MASCHERA "READ-MODIFY-WRITE"     */
/*
/* L'inserimento di un campo di bit richiede una precisione chirurgica:      */
/* dobbiamo modificare una specifica "finestra" di bit all'interno di un     */
/* registro n senza alterare minimamente i bit adiacenti (superiori o      */
/* inferiori).                                                               */
/*
/* Per farlo in sicurezza, l'algoritmo si divide in tre fasi logiche:        */
/* 1. Isolare e ripulire (Azzera i vecchi bit nella finestra di n).         */
/* 2. Allineare (Prende i bit di value,li limita a 'len' e li shifta).*/
/* 3. Innestare (Unisce il registro pulito e il nuovo valore tramite OR).     */
/* */
/* ANALISI DELLA FORMULA BRANCHLESS BLINDATA:                            */
/* mask = -1 ^ (( -1 ^ ((1U << (len & 31)) - 1)) & -(len < 32));             */
/* */
/* - Il trucco del bitwise multiplexer funziona perché sfrutta il segno      */
/* della condizione logica (len < 32). Se è vera (1), il segno meno        */
/* la trasforma nel bitmask totale 0xFFFFFFFF (-1). Se è falsa (0),        */
/* diventa 0x00000000, azzerando il blocco intermedio via AND logica.      */
/* - L'aggiunta di (len & 31) costringe l'hardware a fare un wrapping      */
/* dello shift (32 diventa 0), eliminando l'Undefined Behavior (UB).       */
/* Questo impedisce all'ottimizzatore del compilatore di presumere che       */
/* len sia sempre minore di 32 e di rimuovere arbitrariamente i controlli.*/
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 2. APPLICAZIONI PRATICHE: DRIVER HARDWARE E NETWORKING                     */
/* */
/* Nei sistemi embedded e nei driver del kernel, i registri di controllo     */
/* sono risorse condivise e ultra-dense:                                     */
/* */
/* - REGISTRI DI CONFIGURAZIONE I/O (GPIO): Per cambiare la velocità di un    */
/* pin (es. bit 4-5) senza disattivare l'interruzione del pin accanto      */
/* (bit 6), non si può sovrascrivere l'intero registro. Si deve usare       */
/* questa funzione per fare un inserimento mirato.                         */
/* - PACKING DI PROTOCOLLI AUTOMOBILISTICI (CAN BUS): Nello standard CAN,    */
/* i segnali (es. pressione pedale acceleratore, 5 bit) sono compressi     */
/* ovunque all'interno di un payload di 64 bit. Scrivere un segnale        */
/* richiede l'inserimento esatto nella sua finestra bit-aligned.           */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 3. ARCHITETTURA HARDWARE (PORTES LOGICHE E CPU)                            */
/* */
/* A livello di silicio, l'inserimento di bit viene eseguito in un singolo   */
/* ciclo di clock combinatorio o tramite una macro-operazione interna alla    */
/* ALU (Arithmetic Logic Unit).                                              */
/* */
/* FLUSSO DEI SEGNALI IN HARDWARE:                                           */
/* 1. Generatore di Maschera Termometrica: Riceve len e accende un numero   */
/* di linee di output pari a len. Se len=3, l'output è 00000111.        */
/* 2. Primo Shifter Combinatorio (Barrel Shifter A): Prende la maschera e la */
/* shifta a sinistra di start posizioni.                                */
/* 3. Invertitore di Sbarramento: Un array di 32 porte NOT inverte la        */
/* maschera shiftata, creando la clear_mask.                             */
/* 4. Array di Porte AND: Il registro n originale passa attraverso 32 porte*/
/* AND pilotate dalla clear_mask. La finestra viene letteralmente       */
/* "piallata" a zero alla velocità di propagazione dei transistor.        */
/* 5. Secondo Shifter Combinatorio (Barrel Shifter B): In parallelo, shifta  */
/* il valore value a sinistra di start posizioni.                      */
/* 6. Array di Porte OR Finali: L'output delle AND (n pulito) e l'output del  */
/* secondo shifter vengono uniti bit a bit da 32 porte OR.                */
/* */
/* SUPPORTO NATIVO NEI PROCESSORI MODERNI:                                    */
/* - ARM (Aarch64): Possiede l'istruzione nativa BFI (Bit Field Insert).   */
/* La CPU esegue l'intera funzione in un solo ciclo di clock bypassando    */
/* completamente il calcolo software delle maschere.                       */
/* - x86 (BMI2): Fornisce l'istruzione PDEP (Parallel Bit Deposit), che    */
/* può spargere o inserire gruppi di bit in posizioni arbitrarie           */
/* sotto il controllo di una maschera hardware.                            */
/* -------------------------------------------------------------------------- */

unsigned int bit_field_insert(unsigned int n, unsigned int value, int start, int len)
{
    /* Garanzia di sicurezza: se start invalido o len nullo, non tocchiamo nulla. */
    /* Nei driver hardware, restituire il valore originale inalterato impedisce   */
    /* la corruzione catastrofica dei bit di controllo vicini.                    */
    if (start >= 32 || len <= 0)
        return n;

    /* Generazione maschera branchless corretta e protetta da UB grazie a (len & 31) */
    unsigned int mask = -1 ^ ((-1 ^ ((1U << (len & 31)) - 1)) & -(len < 32));

    /* Tronca il valore in ingresso mantenendo solo i bit consentiti da len */
    value &= mask;

    /* Inverte la maschera posizionata per creare la finestra di azzeramento */
    mask = ~(mask << start);

    /* Fase 1: Ripulitura della finestra sul registro n */
    n &= mask;

    /* Fase 2 e 3: Allineamento del valore e innesto finale tramite OR */
    return n | (value << start);
}

int main(void)
{
    printf("=== TESTING INSERIMENTO CAMPI DI BIT ===\n\n");

    /* Test 1: Il debunking dell'errore della dispensa */
    /* Registro iniziale n = 0xFF (11111111), value = 5 (101), start = 2, len = 3 */
    /* La finestra pulisce i bit 2,3,4. I bit esterni (7,6,5 e 1,0) DEVONO restare a 1. */
    /* Risultato atteso matematicamente: 0xF7 (247 in decimale -> 11110111) */
    unsigned int res1 = bit_field_insert(0xFF, 5, 2, 3);
    printf("Test Dispensa Realistico: Inserisco 5 in 0xFF a pos 2 (len 3)\n");
    printf("-> Risultato Calcolato: %u (Decimale) | %#X (Esadecimale)\n\n", res1, res1);

    /* Test 2: Caso limite len = 32 */
    /* n = 0xAAAAAAA, value = 0x55555555, start = 0, len = 32 */
    /* La maschera deve coprire l'intero registro e rimpiazzarlo completamente. */
    /* Risultato atteso: 0x55555555 */
    unsigned int res2 = bit_field_insert(0xAAAAAAAA, 0x55555555, 0, 32);
    printf("Test Limite 32-bit: Rimpiazzo totale del registro\n");
    printf("-> Risultato Calcolato: %#X (Atteso: 0X55555555)\n\n", res2);

    /* Test 3: Caso limite saturazione len = 33 */
    /* n = 0x0, value = 0xFFFFFFFF, start = 0, len = 33 */
    /* La protezione deve intervenire, forzare la maschera totale senza mandare in crash lo shift. */
    /* Risultato atteso: 0xFFFFFFFF */
    unsigned int res3 = bit_field_insert(0x0, 0xFFFFFFFF, 0, 33);
    printf("Test Limite 33-bit (Overflow len): Protezione UB attivata\n");
    printf("-> Risultato Calcolato: %#X (Atteso: 0XFFFFFFFF)\n\n", res3);

    /* Test 4: Fuori range (start >= 32) */
    /* Tentativo di scrittura illegittimo. Il registro deve tornare indietro intatto. */
    /* Risultato atteso: 0x12345678 */
    unsigned int res4 = bit_field_insert(0x12345678, 0xFF, 35, 4);
    printf("Test Sicurezza (start fuori range): Registro protetto\n");
    printf("-> Risultato Calcolato: %#X (Atteso: 0X12345678)\n", res4);

    return 0;
}
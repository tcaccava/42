#include <stdio.h>
/* ========================================================================== */
/*  BIT_FIELD_EXTRACT.C - REQUISITI DI SISTEMA E OTTIMIZZAZIONE BITWISE        */
/* -------------------------------------------------------------------------- */
/*  1. BACKGROUND TEORICO: ANALISI DELLA MASCHERA E PRESERVAZIONE DELL'ORDINE */
/*                                                                            */
/*  L'estrazione di un campo di bit consiste nell'isolare una "finestra" di   */
/*  lunghezza len partendo dall'indice start (dove 0 è l'LSB, il bit più  */
/*  a destra) e riallineare questo valore in posizione 0.                    */
/*                                                                            */
/*  IMPORTANTE: L'ordine relativo dei bit NON viene alterato o invertito.     */
/*  L'operazione di shift a destra (n >> start) sposta semplicemente i bit    */
/*  bersaglio in fondo al registro. Il bit originariamente a start diventa  */
/*  il bit 0 del risultato finale.                                            */
/*                                                                            */
/*  ANALISI MATEMATICA DELLA FORMULA:                                     */
/*  mask = -1 ^ ((- 1 ^ ((1U << (len & 31)) - 1)) & -(len < 32));             */
/*                                                                            */
/*  Questo approccio implementa un multiplexer booleano branchless:           */
/*                                                                            */
/*  Caso A: len < 32 (Es: len = 8)                                            */
/*  - (len < 32) valuta a 1. Il meno davanti lo trasforma in -1, ovvero       */
/*    la maschera di bit piena 0xFFFFFFFF.                                    */
/*  - La AND logica con 0xFFFFFFFF lascia invariato il termine di sinistra:   */
/*    (-1 ^ ((1U << 8) - 1))                                                  */
/*  - Lo XOR esterno riapplica -1 (0xFFFFFFFF). Per la proprietà dell'XOR:    */
/*    A ^ B ^ A = B. Quindi l'espressione collassa in ((1U << 8) - 1),        */
/*    producendo esattamente 0x000000FF. CORRETTO.                            */
/*                                                                            */
/*  Caso B: len >= 32 (Es: len = 33)                                          */
/*  - (len < 32) valuta a 0. Il segno meno lo mantiene a 0 (0x00000000).      */
/*  - La AND logica azzera completamente il blocco interno.                   */
/*  - Rimane lo XOR esterno: -1 ^ 0 = -1 (ovvero 0xFFFFFFFF). CORRETTO.       */
/*                                                                            */
/*  Perché len & 31 è fondamentale?                                           */
/*  Nello standard ISO C, se si esegue uno shift pari o superiore alla        */
/*  dimensione del tipo di dato (es. 1U << 32), il comportamento è UB.        */
/*  Senza & 31, l'ottimizzatore del compilatore (con -O3) assume che len      */
/*  sia SEMPRE < 32 . Di conseguenza, elimina la protezione -(len < 32)       */
/*  ritenendola ridondante. Forzando il wrapping hardware con & 31,   si      */
/*  elimina l'UB alla radice, costringendo il compilatore a generare un       */
/*  codice assembly deterministico e sicuro.                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*  2. APPLICAZIONI PRATICHE NEL SOFTWARE DI SISTEMA                          */
/*                                                                            */
/*  Nello sviluppo low-level e driver, questa operazione è ovunque:           */
/*                                                                            */
/*  - DECODIFICA DI REGISTRI HARDWARE (MMIO): I chip di rete, le GPU o i      */
/*    microcontrollori mappano lo stato in registri a 32 bit dove i bit 0-3   */
/*    indicano l'ID del comando, i bit 4-11 la dimensione, e i bit 12-15 i    */
/*    flag di errore. Questa funzione estrae i singoli campi in un colpo solo.*/
/*  - PARSING DI PROTOCOLLI DI RETE: I pacchetti IP, TCP o i frame hardware   */
/*    (come il CAN bus automobilistico) compattano i dati al singolo bit per  */
/*    risparmiare banda. Estrarre i flag o i campi di priorità richiede       */
/*    esattamente questa logica.                                              */
/*  - INTERPRETI E VIRTUAL MACHINES: Nel codice di un emulatore o di una VM   */
/*    (es. decodifica di bytecode), le istruzioni compattano Opcode e         */
/*    Registri sorgente/destinazione all'interno di un unico intero.          */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*  3. IMPLEMENTAZIONE HARDWARE  (ASIC / FPGA)                                */
/*                                                                            */
/*  In silicio, l'estrazione di un campo di bit non viene eseguita tramite    */
/*  operazioni aritmetiche sequenziali, ma attraverso una struttura           */
/*  combinatoria pura a latenza fissa.                                        */
/*                                                                            */
/*  L'architettura hardware si divide in due stadi principali cascata:        */
/*                                                                            */
/*  STADIO 1: Il Barrel Shifter (Riallineamento)                              */
/*  - Il registro di input n a 32 bit viene immesso in una rete di            */
/*    multiplexer disposti su log2(32) = 5 stadi.                             */
/*  - Il valore start pilota direttamente le linee di selezione di questi     */
/*    multiplexer.                                                            */
/*    * Stadio 0: Shifta di 1 bit se start[0] è 1                             */
/*    * Stadio 1: Shifta di 2 bit se start[1] è 1                             */
/*    * Stadio 2: Shifta di 4 bit se start[2] è 1                             */
/*    * Stadio 3: Shifta di 8 bit se start[3] è 1                             */
/*    * Stadio 4: Shifta di 16 bit se start[4] è 1                            */
/*  - Questo stadio richiede solo routing fisico e ritardi di propagazione    */
/*    minimi (struttura a porte logiche fissa, nessun clock richiesto).       */
/*                                                                            */
/*  STADIO 2: Generazione della Maschera e AND di sbarramento                 */
/*  - In parallelo allo shift, il valore len viene inviato a un decodificatore */
/*    termometrico (generatore di maschere). Se len=8, il circuito genera     */
/*    32 bit strutturati come 24 zeri a sinistra e 8 uni a destra.            */
/*  - Se len >= 32, il circuito hardware forza le linee direttamente a VCC    */
/*    (tutti 1), bypassando la logica dello shift termometrico per evitare    */
/*    segnali instabili sulle linee di indirizzamento.                        */
/*  - Infine, un array di 32 porte AND elementari unisce l'output del         */
/*    Barrel Shifter e la Maschera generata.                                  */
/*                                                                            */
/*  NATIVE CPU SUPPORT:                                                       */
/*  Data l'importanza critica di questa operazione, quasi tutte le            */
/*  architetture moderne hanno un'unità hardware dedicata interna alla ALU:   */
/*  - x86 (Estensione BMI1): Mappa questa esatta logica nell'istruzione       */
/*    hardware BEXTR (Bit Field Extract).                                     */
/*  - ARM (Aarch32/Aarch64): Implementa nativamente l'istruzione UBFX         */
/*    (Unsigned Bit Field Extract), che esegue l'intera funzione in 1 solo    */
/*    ciclo di clock della CPU.                                               */
/* -------------------------------------------------------------------------- */

unsigned int bit_field_extract(unsigned int n, int start, int len)
{
    // Sanity check per input fuori scala o privi di significato matematico
    if (start >= 32 || len <= 0)
        return 0;

    // Calcolo della maschera branchless protetto da Undefined Behavior tramite wrapping (& 31)
    unsigned int mask = -1 ^ ((- 1 ^ ((1U << (len & 31)) - 1)) & -(len < 32));

    // Shift e isolamento finale del campo selezionato
    return (n >> start) & mask;
}

int main(void)
{
    printf("=== TESTING EXTRAZIONE CAMPI DI BIT ===\n\n");

    // Test 1: Estrazione standard (il tuo esempio iniziale)
    // Registro: 0x00FF0000 -> Bit 16-23 sono 0xFF. Estraiamo 8 bit da posizione 16.
    // Risultato atteso: 0XFF
    printf("Test Standard: %3d bit da pos %2d su 0x00FF0000 -> Calcolato: %#X\n", 8, 16, bit_field_extract(0x00FF0000, 16, 8));

    // Test 2: Caso limite dell'overflow di len (len = 33)
    // Registro: 0xFFFFFFFF. Chiediamo 33 bit (maggiore della dimensione del registro).
    // La maschera deve saturare a 0xFFFFFFFF senza generare crash hardware o UB.
    // Risultato atteso: 0XFFFFFFFF
    printf("Test Limite:   %3d bit da pos %2d su 0xFFFFFFFF -> Calcolato: %#X\n", 33, 0, bit_field_extract(0xFFFFFFFF, 0, 33));

    // Test 3: Caso limite len = 32
    // La maschera deve essere esattamente 0xFFFFFFFF.
    // Risultato atteso: 0XFFFFFFFF
    printf("Test Limite:   %3d bit da pos %2d su 0xFFFFFFFF -> Calcolato: %#X\n", 32, 0, bit_field_extract(0xFFFFFFFF, 0, 32));

    return 0;
}
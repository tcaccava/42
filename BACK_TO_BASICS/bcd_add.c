#include <stdio.h>

/*******************************************************************************
 * TEORIA, APPLICAZIONI E IMPLEMENTAZIONI HARDWARE DEL BCD (BINARY CODED DECIMAL)
 *****************************************************************--------------
 * 
 * 1. BACKGROUND TEORICO
 * 
 * Il BCD (Binary Coded Decimal) Packed comprime due cifre decimali (0-9)
 * all'interno di un singolo byte, sfruttando i due nibble da 4 bit: il nibble
 * superiore per la cifra più significativa, quello inferiore per la meno
 * significativa (es. 0x23 = 23).
 * 
 * Il problema fondamentale quando si sommano due byte in BCD risiede nella
 * natura dell'ALU: la CPU esegue un'addizione binaria pura in base 16 (0xF),
 * non in base 10. Di conseguenza:
 * - Se sommiamo due cifre che superano 9 (es. 9 + 2 = 11), l'ALU produce un
 *   valore esadecimale (0x0B) anziché generare un riporto (carry) decimale
 *   e lasciare il residuo corretto nel nibble.
 * - L'intervallo non valido tra 10 e 15 (0x0A - 0x0F) viene trattato come
 *   continuazione della cifra anziché come overflow della decade.
 * 
 * La correzione matematica richiede di compensare lo scarto tra la base
 * esadecimale e quella decimale (16 - 10 = 6). Quando un nibble supera 9,
 * bisogna aggiungere un offset di correzione pari a 6 (0x06) per forzare
 * il ribaltamento del bit di carry nel nibble superiore.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Sistemi Finanziari e Contabili: I numeri decimali a virgola mobile binaria
 *   (standard IEEE 754) non possono rappresentare esattamente frazioni decimali
 *   come 0.1 o 0.2, introducendo errori di arrotondamento letali nei calcoli
 *   di bilancio, interessi bancari o transazioni di pagamento. Il BCD garantisce
 *   una precisione decimale assoluta, impedendo qualsiasi perdita di centesimi
 *   dovuta alla conversione binaria.
 * - Mainframe ed Enterprise Computing: Architetture storiche e attuali focalizzate
 *   sul transactional processing (come IBM z/Architecture) supportano nativamente
 *   istruzioni aritmetiche decimali per gestire flussi di dati legacy scritti in
 *   COBOL o database transazionali ad alte prestazioni.
 * - Dispositivi Embedded Medicali e Industriali: Display a sette segmenti,
 *   contatori di precisione e strumentazione di misura spesso elaborano
 *   direttamente flussi BCD per evitare l'overhead computazionale e di memoria
 *   delle conversioni continue tra binario puro e stringhe decimali per
 *   l'interfaccia utente.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * - Architettura x86 (Istruzioni Dedicate): Storicamente, i processori x86
 *   integravano istruzioni hardware specifiche come DAA (Decimal Adjust after
 *   Addition) e DAS. Dopo un'addizione binaria standard tra registri contenenti
 *   BCD packed, l'istruzione DAA esaminava i flag di carry (CF) e auxiliary
 *   carry (AF) della CPU, applicando automaticamente la correzione di +6 al
 *   nibble basso o alto in un singolo ciclo di clock. Nelle moderne modalità
 *   a 64-bit (Long Mode), queste istruzioni sono state rimosse dal set nativo,
 *   costringendo i compilatori a gestire la logica via software.
 * - Logica Combinatoria su ASIC / FPGA: Nei circuiti integrati custom o nelle
 *   descrizioni hardware (Verilog/VHDL), il sommatore BCD è implementato tramite
 *   due stadi di adder binari a 4 bit. Il primo stadio esegue la somma grezza.
 *   Un blocco di comparazione logica verifica se il risultato supera 9 o se il
 *   carry è attivo; in caso positivo, un secondo blocco forza l'iniezione del
 *   fattore di correzione +6 mediante una rete di porte AND/OR, propagando
 *   istantaneamente il riporto alla decade successiva senza latenze software.
 *******************************************************************************/
unsigned char bcd_add(unsigned char a, unsigned char b)
{
    unsigned char lo = (a & 0x0F) + (b & 0x0F);
    unsigned char hi = (a >> 4) + (b >> 4);

    // Se il nibble basso supera 9, genera un riporto per il nibble alto
    if (lo > 9)
    {
        lo -= 10;
        hi += 1;
    }

    // Se il nibble alto supera 9, gestiamo il trabocco (es. saturazione a 99)
    if (hi > 9)
    {
        hi = 9;
        lo = 9;
    }

    return (hi << 4) | lo;
}
int main()
{
    printf("%d\n", bcd_add(35, 40));
    printf("%d\n", bcd_add(81, 73));
}
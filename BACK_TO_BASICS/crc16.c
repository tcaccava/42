#include <stdio.h>

// Il CRC-16 (Cyclic Redundancy Check a 16 bit) è un algoritmo di rilevamento degli errori basato sulla teoria dei campi finiti (Galois Fields, specificamente $GF(2))
// e sull'**aritmetica polinomiale senza riporto.
// Nella matematica del CRC, una sequenza di bit non viene vista come un valore numerico classico, ma come una serie di coefficienti di un polinomio. Ad esempio, il byte 10000101
// rappresenta il polinomio: 1 x^7 + 0 x^6 + 0 x^5 + 0 x^4 + 0 x^3 + 1 x^2 + 0 x^1 + 1 x^0 = x^7 + x^2 + 1
// Il cuore dell'algoritmo è il polinomio divisore, fisso e standardizzato, il cui valore esadecimale 0x8005 corrisponde al polinomio binario di 17 bit 11000000000000101 (dove il bit più
// significativo a sinistra, x^16, è implicitamente considerato attivo e spesso omesso nella rappresentazione a 16 bit). Esplicitato in algebra, equivale a:
// x^16 + x^15 + x^2 + 1. Questo specifico schema prende il nome storico di CRC-16-IBM(o CRC-16-ANSI).
// L'algoritmo si compone di 3 meccanismi:
// 1) Moltiplicazione iniziale: il messaggio originale (visto come polinomio) viene idealmente moltiplicato per x^16, operazione che equivale ad aggiungere 16 bit di zeri in coda al messaggio.
// 2) La Divisione polinomiale: il messaggio esteso viene diviso per il polinomio generatore x^16 + x^15 + x^2 + 1). Questa divisione non usa l'aritmetica standard, ma l'aritmetica modulo 2.
// Ciò significa che l'operazione di sottrazione tra bit è interamente sostituita dall'operatore logico XOR (0 - 1 = 1, 1 - 1 = 0, senza prestiti o riporti).
// 3) Il resto (il checksum): il resto di questa divisione matematica è un valore a 16 bit: il CRC. Questo resto viene incollato al posto dei 16 zeri iniziali in coda al messaggio trasmesso.
// 4) La verifica: quando il ricevente riceve l'intero pacchetto (dati + CRC) e lo divide nuovamente per lo stesso polinomio generatore, se i dati non hanno subito alterazioni, il resto della
// divisione deve essere tassativamente zero.
// Il CRC-16 con polinomio 0x8005 è lo standard industriale globale per la protezione dell'integrità dei dati in trasmissioni a pacchetto corto o in ambienti industriali ad alto rumore elettromagnetico:
// -Protocollo Modbus (RTU): e' l'applicazione più celebre. Modbus è lo standard di comunicazione nei sistemi SCADA e nei PLC (Programmable Logic Controller) industriali. Il CRC-16 viene calcolato su ogni
// pacchetto di comando o lettura dei registri per evitare che interferenze elettriche sui cavi seriali (RS-485) attivino macchinari o leggano sensori in modo errato.
// -Standard USB (Universal Serial Bus): utilizzato nei pacchetti di controllo e di configurazione dei token (USB Token Packets). Garantisce che l'identificazione della periferica inserita non venga corrotta
// durante la fase di enumerazione.
// -Bluetooth: utilizzato nello strato di collegamento (Link Layer) per la validazione dei pacchetti di dati a corto raggio.
// Sistemi di storage magnetico legacy: storicamente impiegato da IBM per verificare l'integrità dei settori sui floppy disk e sui primi dischi rigidi.
// A differenza del software, dove la manipolazione bit-a-bit tramite cicli iterativi è computazionalmente inefficiente, il CRC-16 nasce per lavorare in modo nativo e istantaneo sul silicio dei chip.
// Struttura LFSR (Linear Feedback Shift Register): in hardware, il CRC-16 con polinomio 0x8005 si implementa tramite un registro a scorrimento a retroazione lineare. I componenti fisici richiesti sono minimi:
// -16 registri Flip-Flop (Bit da 0 a 15) che mantengono lo stato corrente del resto.
// -porte Logiche XOR posizionate esclusivamente in corrispondenza dei "tap" (i nodi) definiti dal polinomio divisore. Per lo 0x8005, le porte XOR hardware sono cablate prima del Flip-Flop 0 (per l'unità 1),
// prima del Flip-Flop 2 (per x^2) e prima del Flip-Flop 15 (per x^15).
// Il circuito riceve il flusso di dati un bit per ciclo di clock. Ad ogni impulso:
// 1) Il bit in ingresso viene messo in XOR con il bit in uscita dal Flip-Flop più significativo (il bit di feedback).
// 2) Il risultato di questo XOR pilota lo scorrimento laterale dei bit nel registro.
// 3) Se il bit di feedback è 1, la maschera del polinomio viene applicata istantaneamente tramite le porte XOR hardware nei punti specifici del circuito, modificando i bit nello stesso istante in cui scalano di posizione.
// 4) Dopo che l'ultimo bit del messaggio ha attraversato il circuito, i 16 Flip-Flop contengono il CRC-16 finale, pronto per essere letto in parallelo dal bus del sistema.
// Nei microcontrollori per automazione (es. STM32, PIC, chip Automotive)e' integrato un blocco hardware periferico chiamato "CRC Calculation Unit". Questo circuito riceve i dati direttamente tramite DMA (Direct Memory Access)
// senza l'intervento dei registri della CPU. La logica cablata consuma zero cicli macchina del core principale.
// Mancano vere e proprie istruzioni CPU dedicate nelle architetture general-purpose: mentre i processori Intel, AMD o ARM dispongono di istruzioni hardware native per il CRC-32 (richiesto da standard massicci come Ethernet o SATA),
// non possiedono un'istruzione nativa per il CRC-16 0x8005. Nelle CPU dei computer, l'accelerazione hardware di questo algoritmo avviene indirettamente sfruttando le istruzioni hardware di moltiplicazione polinomiale
// carry-less (come PCLMULQDQ in x86 o VMULL in ARM NEON), le quali calcolano la divisione a livello di macro-blocchi paralleli a 64 bit anziché un bit alla volta.

#include <stdio.h>
#include <string.h>

unsigned short ft_crc16(unsigned char *data, size_t len)
{
    // il registro del CRC parte solitamente da 0x0000 o 0xFFFF a seconda del protocollo specifico.
    // per lo standard Modbus/IBM classico si inizializza a 0xFFFF.
    unsigned short crc = 0xFFFF;
    const unsigned short POLINOMIO = 0x8005;

    for (size_t i = 0; i < len; i++)
    {
        // operazione XOR tra il byte corrente e la parte bassa del registro CRC
        crc ^= (unsigned short)data[i];

        // ciclo sui 8 bit del byte appena inserito
        for (int bit = 0; bit < 8; bit++)
        {
            // se il bit più a destra (LSB) è 1, si shifta e si applica il polinomio in XOR
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ POLINOMIO;
            }
            else
            {
                // altrimenti si esegue solo lo scorrimento a destra
                crc >>= 1;
            }
        }
    }

    return crc;
}

int main()
{
    // stringa di test standard
    unsigned char test_data[] = "123456789";
    size_t len = strlen((char *)test_data);

    unsigned short risultato = ft_crc16(test_data, len);

    // stampa il checksum finale in formato esadecimale a 4 cifre
    printf("CRC-16 (0x8005) risultante: 0x%04X\n", risultato);

    return 0;
}
/* ============================================================================
 * DOCUMENTAZIONE TECNICA: ANALISI COMPARATIVA DEL PESO DI HAMMING (POPCNT)
 * ============================================================================
 * * 1. BACKGROUND TEORICO & COMPLESSITÀ COMPUTAZIONALE
 * ----------------------------------------------------------------------------
 * Il calcolo del "Peso di Hamming" (o Population Count) rappresenta il numero
 * di bit impostati a 1 all'interno di una parola binaria. L'efficienza di questo
 * calcolo definisce le performance di interi subset di algoritmi low-level.
 * Il codice sottostante mappa le tre filosofie cardine per risolvere il problema:
 * * A) Approccio Naive Iterativo (count_bits):
 * - Logica: Scansione lineare bit a bit tramite shift sequenziali.
 * - Complessità: O(N) dove N è il numero totale di bit (costante a 8 cicli).
 * - Limiti: Pessimo per la pipeline della CPU. Sebbene privo di branch condizionali
 * interni nel calcolo del risultato grazie all'espressione bitwise, il ciclo
 * for genera overhead di salto (branch) per il decremento del contatore 'i'.
 * * B) Algoritmo di Brian Kernighan:
 * - Logica: Sfrutta la proprietà algebrica del complemento a due. L'operazione
 * `n & (n - 1)` cancella matematicamente il bit meno significativo (LSB)
 * acceso. Sottrarre 1 a un numero binario inverte infatti tutti i bit a destra
 * dell'LSB attivo, incluso l'LSB stesso. L'AND logico azzera quella sezione.
 * - Complessità: O(K) dove K è il numero REALE di bit impostati a 1.
 * - Vantaggio: Nel caso peggiore (0xFF) esegue 8 iterazioni, ma nel caso medio
 * o in flussi di dati sparsi (es. matrici di adiacenza rade) il ciclo converge
 * in pochissimi passi, abbattendo drasticamente l'uso dell'ALU.
 * * C) Approccio SWAR (SIMD Within A Register) (hamming_weight):
 * - Logica: Divide-et-impera parallelo. Il registro non viene visto come un
 * singolo intero, ma come un vettore di parallelismo spaziale.
 * - Maschere Magiche:
 * * 0x55 (01010101): Isola i bit in posizione pari.
 * * 0xAA (10101010): Isola i bit in posizione dispari.
 * * 0x33 (00110011): Isola le coppie di bit adiacenti.
 * * 0xCC (11001100): Isola le coppie di bit superiori.
 * * 0x0F (00001111) / 0xF0 (11110000): Isolano i singoli Nibble (4 bit).
 * - Complessità: O(log N) rispetto alla dimensione del registro. Per un byte
 * richiede esattamente 3 step paralleli stabili, senza ALCUN branch.
 * Il tempo di esecuzione è rigorosamente COSTANTE (determinismo puro), il che
 * lo rende intrinsecamente immune da attacchi di temporizzazione (Side-Channel).
 * ============================================================================
 */

#include <stdio.h>

int count_bits(unsigned char n)
{
    int result = 0;
    for (int i = 7; i >= 0; i--)
        result += ((n >> i) & 1);
    return result;
}
// oppure algoritmo di Brian Kernighan's
//  int count_bits(unsigned char n) {
//      int result = 0;
//      while (n) {
//          n &= n - 1;  // spegne il bit più basso acceso ad ogni iterazione,efficiente per numeri con pochi bit accesi
//          result++;
//      }
//      return result;
//  }

// popcount,conta il numero di bit accesi,chiamato cosi in onore di Richard Hamming

unsigned char hamming_weight(unsigned char n)
{
    // approccio SWAR puro
    unsigned char even_bits = n & 0b01010101; // bit in posizione 0 2 4 6 con 0x55
    unsigned char odd_bits = n & 0b10101010;  // bit in posizione 1 3 5 7 con 0xAA
    odd_bits = odd_bits >> 1;                 // allineo i bit dispari ai pari
    unsigned char sum = odd_bits + even_bits; // adesso ogni coppia di bits contiene il numero di bit accesi di quella coppia,quindi o 00, o 01, 0 10
    // replico lo stesso concetto prima per coppie di coppie,poi per i due nibble
    unsigned low_couples = sum & 0b00110011;      // isolo le coppie basse 0x33
    unsigned high_couples = sum & 0b11001100;     // isolo le coppie alte 0x
    high_couples = high_couples >> 2;             // allineo le alte alle basse
    sum = low_couples + high_couples;             // adesso ogni nibble vale 0,1,2,3,4
    unsigned char low_nibble = sum & 0b00001111;  // 0x0F
    unsigned char high_nibble = sum & 0b11110000; // 0xF0
    high_nibble = high_nibble >> 4;
    return sum = low_nibble + high_nibble;
}
// oppure compressa
// unsigned char hamming_weight(unsigned char n) {
//     n = (n & 0x55) + ((n & 0xAA) >> 1);
//     n = (n & 0x33) + ((n & 0xCC) >> 2);
//     n = (n & 0x0F) + ((n & 0xF0) >> 4);
//     return n;
// }
// seguiamo il flusso sul numero 01101100
// bit pari con maschera 0x55 = 01000100
// bit dispari con maschera 0xAA = 00101000
// shifto i bit dispari di 1 = 00010100
// sommo 01011000
// isolo le coppie basse con la maschera & 0x33 = 00010000
// coppie alte con maschera & 0xCC 01001000
// shifto le maschere alte di 2 a destra = 00010010
// sommo le coppie 00100010
// nibble basso con 0x0f = 00000010
// nibble alto con 0xF0 = 00100000
// shifto il nibble alto di 4 a destra = 00000010
// sommo e ottengo la somma finale = 00000100,cioe' 4
// int main() {
//     printf("%d\n", hamming_weight(0b00110101));
//     printf("%d\n", hamming_weight(0b11111111));
//     printf("%d\n", hamming_weight(0b11111001));
//     printf("%d\n", count_bits(255)); // 8
//     printf("%d\n", count_bits(128)); // 1
//     printf("%d\n", count_bits(127));
// }

/* ============================================================================
 * 2. APPLICAZIONI PRATICHE NEL REAL-WORLD SOFTWARE
 * ----------------------------------------------------------------------------
 * Calcolare quanti bit sono accesi serve in produzione per gestire strutture dati
 * ad altissima densità informativa:
 * * - Distanza di Hamming (Teoria dell'Informazione & Telecomunicazioni):
 * Utilizzata per rilevare errori nei codici di trasmissione (es. Reed-Solomon o
 * codici a parità). La distanza tra due stringhe di bit si calcola facendo lo
 * XOR dei due flussi e applicando il popcount sul risultato. Più il peso è basso,
 * meno alterazioni ha subito il pacchetto sulla rete.
 * * - Strutture Dati Succinte (Rank & Select):
 * Nei moderni motori di ricerca o database colonnari compressi, le informazioni
 * vengono indicizzate tramite enormi vettori di bit (Bitmap). Per trovare la
 * posizione assoluta di un dato senza decomprimere l'intero indice, si usa
 * l'operazione `Rank(i)`, implementata proprio tramite un popcount mascherato
 * fino all'indice `i`. Velocità di esecuzione istantanea garantita.
 * * - Motori di Scacchi (Bitboard Evaluation):
 * Le posizioni dei pezzi sulla scacchiera sono salvate in interi a 64 bit (Bitboards).
 * Sapere quanti pezzi controllano una diagonale o valutare la densità di pedoni
 * richiede chiamate massive a funzioni di popcount. Ottimizzare questa funzione
 * significa aumentare i nodi al secondo calcolati dall'algoritmo di alpha-beta pruning.
 * * ============================================================================
 * 3. IMPLEMENTAZIONE HARDWARE
 * ----------------------------------------------------------------------------
 * Se un programmatore software usa trucchi SWAR per simulare il parallelismo,
 * l'ingegnere elettronico progetta circuiti fisici asincroni e combinatori.
 * * - Alberi di Somma Combinatoria (Wallace / Dadda Trees):
 * A livello di transistor (FPGA/ASIC), il popcount non usa clock né registri
 * di scorrimento. Viene implementato cablando in cascata moduli di Full Adder
 * (Sommatori Completi 3:2). Un Full Adder prende 3 linee di bit in ingresso
 * e restituisce 2 linee di output (Sum e Carry), interpretate come un numero
 * binario a 2 bit. Sfruttando questa compressione in un albero logaritmico,
 * il segnale elettrico attraversa le porte logiche (AND, OR, XOR) stabilizzandosi
 * sul valore del conteggio finale in una frazione minima di ciclo d'orologio.
 * * - Istruzioni Native della CPU:
 * Dato l'uso intensivo di questa operazione, i produttori di hardware hanno
 * smesso di delegare il calcolo al software:
 * * x86_64: Intel introduce l'istruzione assembler nativa `POPCNT` nel 2008
 * (SSE4.2). Mappa direttamente l'albero combinatorio hardware sui registri.
 * * ARM: Integra l'istruzione vettoriale `VCNT` nei registri NEON/Advanced SIMD.
 * * Nota di compilazione: Se compili questo file con compilatori moderni (GCC/Clang)
 * impostando i flag `-O3 -march=native`, l'ottimizzatore riconoscerà il pattern
 * SWAR della funzione `hamming_weight` o il ciclo di `count_bits`, piallerà tutto
 * il mio codice scritto a mano e lo sostituirà direttamente con una singola
 * istruzione hardware nativa della CPU, eseguita in un solo ciclo di clock.
 * ============================================================================
 */
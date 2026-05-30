#include <stdio.h>
// Linear Feedback Shift register a 8 bit,usato in crittografia(cifrari a flusso) e generatori hardware di numeri pseudocasuali,ma anche CRC(cyclic redundancy check),
// cioe' algoritmi per controlli errori in rete e storage, e spread spectrum per comunicazioni wireless
// Implementa un registro a scorrimento con feedback lineare a 8 bit : a ogni passo shifta di 1 a sinistra e inserisce come bit basso(bit 0) lo XOR dei bit
// in posizione 7 5 4 3. I bit di feedback sono chiamati tap: la scelta del tap determina la lunghezza del ciclo. I tap 7 5 4 3 producono un ciclo massimo di 255 valori
// prima di ripetersi.
// Un LFSR a 8 bit è un automa a stati finiti che si evolve nel tempo. Lo stato del registro è rappresentato da un singolo byte (il seed iniziale; ad ogni passo (o iterazione),
// l'algoritmo esegue due azioni congiunte:
// 1)Calcolo del bit di feedback: un'operazione logica XOR tra bit specifici (chiamati taps) dello stato corrente. Per un LFSR a 8 bit a ciclo massimo (periodo pari a 2^8 - 1 = 255
// stati prima della ripetizione), un polinomio generatore standard molto comune è: x^8 + x^6 + x^5 + x^4 + 1
// Questo significa che il bit di feedback viene generato mettendo in XOR i bit nelle posizioni corrispondenti a questi esponenti.
// 2)Scorrimento (shift): lo stato viene shiftato a destra (o a sinistra) di 1 bit, e il bit di feedback appena calcolato viene inserito nella posizione lasciata vuota (il bit più significativo o meno significativo).
// Mentre la funzione base dell'LFSR calcola la transizione da uno stato S_t allo stato successivo S_{t+1}, la funzione di sequenza agisce come un generatore di numeri pseudo-casuali (PRNG)
// macroscopico. L'algoritmo prende lo stato iniziale (seed), applica la trasformazione hardware/software per generare il nuovo stato, e salva l'intero byte risultante all'interno di
// un vettore di output. Questo processo viene ripetuto ciclicamente per n volte, dove ogni byte generato dipende strettamente e deterministicamente dal byte precedente.
// Se il seed iniziale è 0, l'algoritmo si blocca producendo una sequenza infinita di soli zeri (poiché 0 XOR 0 = 0). Se il seed è valido (qualsiasi valore tra 1 e 255), e l'LFSR usa
// un polinomio primitivo a ciclo massimo, la sequenza di byte generata mostrerà un'eccellente uniformità statistica, simulando il rumore bianco, fino al raggiungimento del 255° byte, punto in cui l'intera sequenza
// ricomincerà a ripetersi identica.
// La generazione di sequenze tramite LFSR a 8 o più bit ha utilizzi cruciali ovunque serva determinismo combinato con la parvenza di casualità:
// -Scrambling e riduzione delle interferenze (EMI): nei protocolli di trasmissione seriale ad altissima velocità (come USB 3.0, PCIe, SATA, o i canali video HDMI), trasmettere lunghe stringhe di
// bit identici (es. tutti 1 o tutti 0) crea picchi di radiazione elettromagnetica che possono disturbare i circuiti vicini. La sequenza di un LFSR viene combinata in XOR con i dati da trasmettere per "mescolarli" (scrambling)
// e distribuire lo spettro di frequenza, riducendo le interferenze sui cavi. Il ricevente userà la stessa sequenza LFSR per decodificare i dati originali.
// -BILT (Built-In Logic Test) e test hardware: nei circuiti integrati moderni e nelle FPGA, per verificare che le linee fisiche di comunicazione e i pin non abbiano cortocircuiti o difetti di fabbricazione, si iniettano
// sequenze pseudo-casuali generate da LFSR (chiamate PRBS - Pseudo-Random Binary Sequence). Il chip ricevente rigenera la stessa sequenza e la confronta: se un solo bit differisce, il test hardware fallisce.
// Cifratura a flusso (Stream Ciphers Leggeri): utilizzato in vecchi standard di crittografia wireless o RFID a bassissimo consumo energetico, dove la sequenza di byte generata dall'LFSR funge da keystream
// da applicare in XOR al testo in chiaro, in modo del tutto analogo a quanto visto per l'RC4.
// L'LFSR è una struttura intrinsecamente hardware che soffre molto la trasposizione software, poiché le CPU lavorano a blocchi di parole (32/64 bit) e non gradiscono i calcoli bit-a-bit.
// Architettura a livello di Silicio (FPGA/ASIC):in hardware la generazione di una sequenza LFSR è istantanea e parallela. Il circuito è composto da un banco di 8 Flip-Flop di tipo D collegati in cascata.
// Ad ogni ciclo di clock, il bit scorre al Flip-Flop successivo, mentre le porte XOR cablate sui taps iniettano il feedback. L'output a 8 bit (il byte della sequenza) è immediatamente disponibile sulle linee in parallelo ad ogni
// singolo ciclo di clock.
// Le CPU general-purpose (Intel, AMD, ARM) non possiedono istruzioni hardware native per far avanzare un LFSR o generare una sequenza PRBS, in quanto l'overhead software di shift e mascheramento bitwise è considerato accettabile
// per i volumi di calcolo standard dei computer desktop o server.
// Molti microcontrollori moderni (come quelli usati nell'automazione industriale o nell'automotive) integrano un modulo hardware periferico generatore di numeri casuali (RNG) o unità di calcolo CRC che possono essere riconfigurate
// via hardware per agire come LFSR, generando sequenze di byte ad alta velocità tramite canali DMA, senza consumare cicli di calcolo della CPU principale.

unsigned char lfsr(unsigned char n)
{
    // calcola il bit di feedback portando i bit 7, 5, 4, 3 nella posizione 0
    unsigned char bit = ((n >> 7) ^ (n >> 5) ^ (n >> 4) ^ (n >> 3)) & 1;

    // shifta a sinistra di 1 e inserisce il bit di feedback nella posizione più bassa (bit 0)
    return (n << 1) | bit;
}

// Per mappare perfettamente il polinomio x^8 + x^6 + x^5 + x^4 + 1 su un registro che shifta a destra, la convenzione hardware stabilisce che i bit di feedback vengano presi partendo dai bit che "escono" dal registro (i bit meno significativi) o
// dalle posizioni canoniche invertite.Per mantenere lo shift a destra (>> 1) e avere un ciclo massimo reale di 255 valori (senza che il codice collassi), i tap corretti da mettere in XOR per un registro a 8 bit sono i bit nelle
// posizioni 0, 2, 3, 4 (che corrispondono esattamente ai tap speculari di 7, 5, 4, 3 quando si inverte la direzione del flusso nel silicio).

// unsigned char lfsr_right(unsigned char n)
// {
//     // calcola il bit di feedback usando i bit nelle posizioni 0, 2, 3, 4
//     // questo set di tap garantisce il ciclo massimo di 255 combinazioni stabili
//     unsigned char bit = (n ^ (n >> 2) ^ (n >> 3) ^ (n >> 4)) & 1;

//     // shifta a destra di 1 e inietta il bit di feedback nella posizione più alta (bit 7)
//     return (n >> 1) | (bit << 7);
// }

void lfsr_sequence(unsigned char seed, unsigned char *out, size_t n)
{
    if (!seed)
        return; // seed 0 blocca l'LFSR
    unsigned char state = seed;
    for (size_t i = 0; i < n; i++)
    {
        state = lfsr(state);
        out[i] = state;
    }
}

int main()
{
    printf("%d\n", lfsr(0b10110001)); // 99
}
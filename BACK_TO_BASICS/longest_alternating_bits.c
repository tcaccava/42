#include <stdio.h>
#include "./keep_highest_bit.c"
#include "./popcount.c"
// Trova la sequenza più lunga di bit alternati (010101 o 101010) dentro un numero a 32 bit. Non esiste un algoritmo noto standard.
// int longest_alternating_bits(unsigned int n)
// {
//     // count e max partono da 1 perche',per tutti i numeri a 32bit diversi da 0,ci sono 31 transizioni possibili(cioe' i passaggi da un bit al successivo che controllo nel ciclo for).
//     // quindi se per esempio ho 2 transizioni valide(1 -> 0 -> 1),i bit alternati sono 3. Quindi la lunghezza della sequenza alternante sara' sempre uguale al numero di transizioni valide + 1.
//     // inoltre qualsiasi numeri diverso da 0 ha almeno un bit isolato,che da solo costituisce una sequenza di specchio lunga almeno 2(se l'1 e' agli estremi) o 3 nel mezzo(010)
//     int count = 1;
//     int max = 1;
//     for (int i = 0; i < 31; i++) // itero lungo tutta la sequenza binaria
//     {
//         // total branchless: se il bit attuale e' diverso dal successivo,aggiungo 1 a count,altrimenti resetto count a 1,perche nel momento in cui la sequenza si spessa il bit corrente diventa automaticamente il primo bit di una nuova potenziale sequenza
//         count += (1 - count) ^ ((1 - count) ^ 1) & -(((n >> i) & 1) ^ ((n >> (i + 1)) & 1));
//         // anche qui branchless: se count e' > di max,max diventa count,altrimenti rimane immodificato
//         max = max ^ ((count ^ max) & -(count > max));
//     }
//     return max * (!!n); // ritorno max, o 0 se n e' 0 (il doppio operatore not riduce qualunque variabile ad una condizione booleana)
// }
// Esiste anche un approccio totalmente SWAR ,estremamente complesso e legato ad una profonda comprensione dei bit trick di Hacker's Delight,nello specifico:
// 1) basicamente una alternanza di bit significa che un bit e' differente da quello adiacente,e possiamo estendere questo concetto all' intero numero
// usando il confronto tra l'intero numero stesso e la sua traslazione a destra di un unita',raccogliendo la differenza con l'operatore XOR(basicamente stiamo convertendo il numero binario nel suo codice Gray)
// 2) il numero ottenuto dalla precedente operazione e' sovrastimato,perche' contiene l'informazione relativa a 32 transizioni interne,ma come abbiamo detto a noi interessano le 31 transizioni possibili
// quindi usiamo la maschera 0x7FFFFFFF,cioe' basicamente una striscia di 31 bit accesi a partire dall'lsb per isolare le transizioni interne reali a 32 bit. In questo modo
// se n era 0x55555555,cioe' 32 bit alternati,diff sara' 0xFFFFFFFF,cioe 32 bit accesi,e transitions diventera' 0x7FFFFFFF
// 3)Adesso il problema si e' trasformato semplicemente nel "trova la piu' lunga sequenza di bit accesi contigui",e di nuovo ci viene in aiuto Hacker Delight col suo algoritmo SWAR
// che sfrutta il fatto che l'AND tra un numero e se stesso shiftato a destra di 1(aggiungendo cosi uno zero a sinistra),accorcia tutte le catene di 1 accesi di una unita',dovunque esse siano,
// e quindi il ciclo while iterera' esattamente un numero di volte pari a quello della catena piu' lunga. Restituiremo quel valore addizionato di 1 per la questione che il numero di transizioni e' pari alla lunghezza della
// sequenza + 1, o zero se il numero e' 0.
// Algoritmi di questo tipo non sono solo speculazioni accademiche, ma pilastri in contesti specifici:
// -Codifica Manchester (telecomunicazioni): nelle trasmissioni Ethernet o RF analogiche, i dati non vengono inviati come semplici 0 e 1 stabili, ma viene forzata una transizione di bit al centro di ogni ciclo di clock per
// mantenere i trasmettitori sincronizzati (es. 0 diventa 01 e 1 diventa 10). Monitorare la lunghezza delle sequenze alternate serve a intercettare il "Preambolo" di sincronizzazione o a rilevare perdite di segnale e rumore di linea.
// -Test di randomicità (crittografia): nei pacchetti di test statistici per i generatori di numeri casuali (come i test NIST SP 800-22), esiste il Runs Test. Se un hardware RNG (Random Number Generator) inizia a sputare stringhe con
// troppi bit alternati (o troppi bit uguali), significa che il sistema è diventato prevedibile. Questo algoritmo calcola istantaneamente se l'entropia è compromessa.
// -Compressione dati: e' la base per ottimizzare le varianti del Run-Length Encoding (RLE) applicato a bitstream ad alta frequenza di oscillazione.
int longest_alternating_bits(unsigned int n)
{
    unsigned int diff = n ^ (n >> 1);
    unsigned int transitions = diff & (0x7FFFFFFF);
    int count = 0;
    while (transitions)
    {
        transitions &= (transitions >> 1);
        count++;
    }
    return (count + 1) * (!!n);
}
int main()
{
    printf("%d %d %d %d\n", longest_alternating_bits(0x55555555), longest_alternating_bits(0x00555500), longest_alternating_bits(0xAAA0AAAA), longest_alternating_bits(0xAAAAAAA0)); // 32 17 17 28
}

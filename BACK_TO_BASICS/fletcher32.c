#include <stdio.h>

// Il Fletcher-32 è il re indiscusso dell'efficienza ingegneristica. È l'algoritmo nato per chi ha bisogno di un controllo d'integrità
// formidabile ma non può permettersi di bruciare cicli di clock preziosi della CPU in pesanti calcoli polinomiali (come i CRC).
// Background Teorico e Matematico
// Un semplice checksum aritmetico (la somma pura di tutti i byte) ha un difetto drammatico: è cieco all'ordine dei dati. Se trasmetti la
// stringa "AB" (valori 65 e 66, somma 131) o la stringa invertita "BA", un checksum normale restituirà lo stesso identico risultato.
// Non rileverà lo scambio.
// John G. Fletcher nel 1982 risolse il problema introducendo la dipendenza posizionale attraverso due accumulatori lineari accoppiati.
// L'algoritmo Fletcher-32 lavora su blocchi di dati a 16 bit (unsigned short) e gestisce due accumulatori a 16 bit, che chiameremo A e B.
// Entrambi vengono inizializzati a 0. Per ogni parola di dati D_i all'interno del messaggio, gli accumulatori vengono aggiornati in cascata
// seguendo queste formule cicliche:
// A = (A + D_i) % 65535
// B = (B + A) % 65535
// Al termine dell'elaborazione di tutte le parole del messaggio, i due accumulatori a 16 bit vengono fusi in un unico verdetto a 32 bit,
// dove B occupa la meta' alta della parola e A quella bassa:
// Perché rileva le trasposizioni? La genialità sta nell'accumulatore B. Essendo la somma progressiva di tutti i valori assunti da A,
// matematicamente B diventa una somma pesata dei dati di input.
// Se espandiamo l'equazione per un messaggio di 3 parole (D_1, D_2, D_3), otteniamo:
// A = D_1 + D_2 + D_3
// B = 3D_1 + 2D_2 + D_3
// La prima parola trasmessa pesa tre volte più dell'ultima. Se scambi di posto D_1 e D_2, il valore finale di A rimarrà invariato, ma il valore di B
//  cambierà drasticamente. Questo garantisce una rilevazione quasi totale degli errori di inversione o spostamento dei dati.
// La scelta di usare come divisore 65535 (ovvero 2^16 -1) invece del classico 2^16 (che sarebbe il naturale overflow del tipo di dato a 16 bit) non è un capriccio.
// In aritmetica modulare, lavorare in modulo 2^n-1 equivale a usare il sistema di rappresentazione dei numeri in complemento a uno.
// Questo approccio offre un vantaggio crittografico enorme: la propagazione degli zeri. Se usassimo il modulo 2^ 16, un blocco intero di zeri inserito nel messaggio
// non cambierebbe di una virgola gli accumulatori. Usando 2^ 16-1, lo zero acquisisce proprietà di controllo migliori, riducendo le probabilità che
// sequenze di bit nulli mascherino un errore di trasmissione.
// Il Fletcher-32 (e il suo cugino stretto Adler-32, che usa come modulo il numero primo 65521) è lo standard di fatto ovunque serva velocità pura unita a un'eccellente
// affidabilità.

// Protocolli di Rete (OSPF): il protocollo di routing Open Shortest Path First (OSPF), che governa l'instradamento dei pacchetti all'interno delle grandi reti aziendali e
// infrastrutture internet, usa il Fletcher checksum per validare i pacchetti LSA (Link State Advertisements). In queste architetture, i router devono validare migliaia
// di pacchetti al secondo: un calcolo CRC software manderebbe la CPU in sofferenza, mentre Fletcher corre via liscio.
// Storage e File System di fascia Enterprise (ZFS): il file system open-source ZFS (creato originariamente da Sun Microsystems), pilastro della memorizzazione dati industriale
// per la sua capacità di auto-riparazione dal degrado dei dati (silent data corruption), adotta varianti del Fletcher checksum (Fletcher-2 e Fletcher-4) per proteggere
// l'integrità dei blocchi logici sul disco. Offre un throughput di validazione nettamente superiore rispetto a funzioni hash crittografiche come SHA, saturando la banda dei
// dischi NVMe senza appesantire il sistema.

// Se dovessimo implementare il Fletcher-32 in un chip hardware (FPGA o ASIC) usando una vera operazione di divisione logica per calcolare il modulo 65535, il circuito risulterebbe enorme, lento e costoso.
// La divisione in silicio richiede una quantità spropositata di porte logiche.
// L'hardware aggira questo ostacolo sfruttando una proprietà matematica meravigliosa del modulo 2^n-1, implementando il trucco del Carry End-Around*(riporto circolare).
// Il circuito di Addizione Circolare: quando esegui la somma algebrica tra l'accumulatore a 16 bit e la parola di dati, il risultato può generare un bit di riporto (il 17° bit, o overflow bit).
// In modulo 2^16}, quel bit verrebbe semplicemente ignorato e troncato. In modulo 2^16 -1, il valore 2^16  è matematicamente equivalente a 1 (infatti 65536 % 65535 = 1).
// Di conseguenza, per fare il modulo in hardware a costo zero, il progettista deve solo prendere quel 17° bit di riporto che esce dalla testa dell'addicatore e riportarlo all'ingresso,
// sommandolo direttamente al bit meno significativo (posizione 0).
// In un chip dedicato:
// Il calcolo viene organizzato in una struttura a pipeline a due stadi: il primo stadio calcola la nuova configurazione di A, e immediatamente il secondo stadio (nel ciclo di clock successivo) usa il valore
// appena stabilizzato per calcolare B. Questo consente al circuito di accettare una nuova parola di dati a 16 bit ad ogni singolo ciclo di clock, operando in tempo reale alla frequenza nativa del bus
// di trasmissione (es. PCIe o interfaccia di rete), mantenendo l'area di silicio occupata incredibilmente ridotta (bastano due registri e due sommatori con riporto circolare).

unsigned int fletcher32(unsigned short *data, size_t len)
{
    unsigned int a = 0; // anche se a e b debbono essere accumulatori a 16 bit,usiamo int e non short perche' quando faremo le somme avremo tutto lo spazio per far sbordare il numero senza perdere bit per overflow,prima di applicare il modulo
    unsigned int b = 0;
    for (int i = 0; i < len; i++)
    {
        a += data[i];
        a %= 65535;
        b += a;
        b %= 65535;
    }
    return (b << 16) | a;
}

// L'operazione di modulo (% 65535) ad ogni singolo giro del ciclo è pesantissima per la CPU (perché dietro le quinte equivale a una divisione).
// Visto che usiamo A e B come variabili capienti a 32 bit, e i dati in input sono piccoli (16 bit), si puo' usare un trucco micidiale: accumulare le somme
// liberamente senza fare il modulo ad ogni giro. Matematicamente, posso fare fino a 360 giri di ciclo sommando i dati dentro A e B in modo bruto (A += data[i]; B += A;)
// senza che B rischi mai di andare in overflow oltre i 32 bit. Quindi, nella pratica industriale, si fa un ciclo interno che macina i dati a blocchi (es. 350 parole alla volta),
// e si applica il modulo % 65535 una sola volta alla fine del blocco, invece che ad ogni singolo elemento. Le prestazioni schizzano alle stelle.
// unsigned int fletcher32(unsigned short *data, size_t len)
// {
//     unsigned int a = 0;
//     unsigned int b = 0;

//     while (len > 0)
//     {
//         // Scegliamo un blocco di massimo 350 parole per evitare l'overflow di B
//         size_t tlen = (len > 350) ? 350 : len;
//         len -= tlen;

//         // Ciclo interno veloce: solo somme brute, ZERO divisioni/moduli
//         for (size_t i = 0; i < tlen; i++)
//         {
//             a += *data++;
//             b += a;
//         }

//         // Il modulo pesante viene eseguito UNA SOLA VOLTA alla fine del blocco
//         a %= 65535;
//         b %= 65535;
//     }

//     return (b << 16) | a;
// }
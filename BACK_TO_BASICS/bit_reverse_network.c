#include <stdio.h>
/*
Si tratta della generalizzazione logaritmica dell'inversione dei bit: questo problema non si affronta pensando a un "ciclo che scorre i bit", 
ma a una rete combinatoria a stadi (un Interconnection Network).
L'inversione classica dei bit (specchiare una stringa binaria) applicata a un intero a 32 bit richiede un numero fisso di passaggi geometrici basati sulla strategia del divide et impera. 
Per invertire 32 bit, servono esattamente log_2(32) = 5 stadi.
La funzione bit_reverse_network generalizza questo concetto introducendo il parametro stages. Questo significa che non sei obbligato a eseguire sempre 
tutti e 5 i livelli di inversione, ma puoi fermare la granularità della rete a un determinato livello di scomposizione, o modellare la dimensione del campo di bit da invertire 
(ad esempio, un'FFT a N punti lavora su un numero di bit pari a log_2(N)).
Il principio matematico si basa sull'algoritmo di Cooley-Tukey per la FFT (Fast Fourier Transform). Quando l'algoritmo divide ricorsivamente il segnale in 
componenti di indice pari e dispari, l'ordine dei dati nel tempo viene stravolto. L'indice finale in cui va a posizionarsi un dato non è sequenziale, 
ma corrisponde all'esatto valore binario del suo indice originario letto al contrario (Bit-Reversal Permutation).
Una rete a stadi risolve questa permutazione applicando in sequenza maschere bitwise e shift che dimezzano (o raddoppiano) la loro ampiezza a ogni passaggio, 
imitando la struttura di un grafo a farfalla (Butterfly Network).

Applicazioni pratiche:
-Algoritmi FFT In-Place: senza la permutazione bit-reverse, il calcolo della FFT richiederebbe l'allocazione di array specchio temporanei, portando la complessità spaziale a O(N). 
Riordinando i dati attraverso la rete di bit-reversal prima o dopo i round matematici, i coefficienti possono essere sovrascritti direttamente negli stessi registri di memoria (in-place), 
ottimizzando l'uso della cache L1/L2.
-Interleaving nelle comunicazioni digitali (Wi-Fi / LTE / 5G): durante la trasmissione radio, i disturbi atmosferici tendono a distruggere blocchi di bit consecutivi (burst errors). 
Facendo passare il flusso di dati attraverso una rete di bit-reversal parziale (impostando un numero specifico di stages), i bit consecutivi vengono sparpagliati nel tempo prima dell'invio. 
Se un'interferenza distrugge quattro bit consecutivi nell'aria, una volta riordinati dal ricevitore i guasti risulteranno isolati e distanti tra loro, permettendo ai codici di correzione errore (FEC) 
di ricostruire il segnale senza perdite.

Sul silicio delle unità di elaborazione dei segnali (DSP) o delle FPGA, la gestione della bit-reversal assume due forme radicalmente diverse a seconda della flessibilità richiesta:
-A costo zero (Hardwired Permutation): se la dimensione dell'FFT è fissa e nota a priori, l'inversione dei bit non richiede porte logiche, transistor o cicli di clock. Viene realizzata semplicemente 
incrociando i fili di rame del bus che collegano l'uscita di un registro all'ingresso del successivo (il filo del bit 0 viene saldato sulla linea del bit 31). Il ritardo di propagazione è pari a zero nanosecondi.
-Rete di Benes / Butterfly dinamica: quando il chip deve supportare configurazioni variabili (es. un modem che passa dinamicamente da FFT a 512 punti a FFT a 2048 punti), l'hardware implementa la struttura richiesta. 
Ogni stage è un livello fisico di multiplexer controllati da una linea di selezione. Se lo stage è attivo (1), il multiplexer scambia i canali (cross-over); se è disattivato (0), lascia passare i bit 
dritti (pass-through).
*/

   unsigned int bit_reverse_network(unsigned int n, int stages)
{
    int          i;
    unsigned int shift;
    unsigned int mask;
    int          k;

    // Protezione: su 32 bit, massimo 5 stadi (1, 2, 4, 8, 16)
    if (stages > 5)
        stages = 5;

    i = 0;
    while (i < stages)
    {
        // Calcoliamo la distanza di shift per questo stadio (1, 2, 4, 8, 16)
        shift = 1U << i;

        // Piantiamo il "seme" della maschera (es. se shift è 4, mask diventa 0x0F)
        mask = (1U << shift) - 1;

        // Duplichiamo il seme per coprire tutti i 32 bit del registro
        k = shift << 1;
        while (k < 32)
        {
            mask |= (mask << k);
            k <<= 2;
        }

        // Eseguiamo lo scambio a farfalla usando la maschera dinamica appena creata
        n = ((n & mask) << shift) | ((n & ~mask) >> shift);

        i++;
    }
    return (n);
}

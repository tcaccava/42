#include <stdio.h>

// Il Bitonic Sort non è un semplice algoritmo di ordinamento basato su confronti sequenziali (come il QuickSort o il MergeSort),
// ma appartiene alla categoria delle Sorting Networks (reti di ordinamento).
// Nelle CPU tradizionali, algoritmi asimmetrici come il QuickSort dominano la scena grazie alla loro velocità media.
// Ma quando ci si sposta sull'hardware massivamente parallelo (come le GPU o i chip FPGA), il QuickSort fallisce miseramente
// perché i suoi rami condizionali spezzano il parallelismo. Il Bitonic Sort, al contrario, esegue sempre lo stesso numero fisso
// di passaggi e confronti, a prescindere da come sono distribuiti i dati. È puro determinismo hardware applicato all'ordinamento.
// Il motore dell'intero algoritmo si basa sul concetto di sequenza bitonica. Una sequenza di numeri si definisce bitonica se è composta
// da due parti: una strettamente monotona crescente e una strettamente monotona decrescente (o viceversa).
// Ad esempio, la sequenza [1, 3, 5, 7, 6, 4, 2, 0] è bitonica.
// Il teorema cardine di Ken Batcher (l'inventore dell'algoritmo) dimostra che se prendi una sequenza bitonica di lunghezza N (dove N è una potenza di 2)
// e confronti l'elemento i con l'elemento i + N/2, scambiandoli in modo che il più piccolo vada a sinistra e il più grande a destra, ottieni due proprietà incredibili:
// 1)Entrambe le sotto-sequenze risultanti (quella di sinistra e quella di destra) rimangono bitoniche.
// 2)Ogni singolo elemento della sotto-sequenza di sinistra è più piccolo di qualsiasi elemento della sotto-sequenza di destra.
// Questo passaggio si chiama Bitonic Merge e ripetendolo ricorsivamente sulle sotto-sequenze, l'intero array si ritrova ordinato.
// Il Bitonic Sort lavora "dal basso verso l'alto" (bottom-up). Se l'array di partenza è totalmente disordinato, non è bitonico.
// L'algoritmo deve quindi prima *costruire* sequenze bitoniche sempre più grandi per poi fonderle.
// 1)Fase di Costruzione:prende coppie di elementi adiacenti. Ordina la prima coppia in senso crescente e la seconda in senso decrescente. Unendo queste due coppie, ottieni una sequenza bitonica di 4 elementi.
// 2)Fase di Espansione:prende le sequenze di 4 elementi, ne ordina una in senso crescente e quella affianco in senso decrescente, creando una sequenza bitonica di 8 elementi.
// 3)Fase di Fusione Finale: continua così finché l'intero array non diventa un'unica gigantesca sequenza bitonica, che viene infine fusa in un array completamente ordinato tramite l'ultimo Bitonic Merge.
// In una GPU, il costo computazionale più alto non è l'operazione matematica, ma il salto condizionale. Se i thread di un blocco di calcolo prendono strade
// diverse (divergenza dei thread), le prestazioni crollano.
// Il Bitonic Sort elimina questo problema alla radice perché la struttura dei confronti è pre-determinata a livello di bit. L'indice dell'elemento con cui fare il confronto viene calcolato tramite operazioni bitwise
// (usando operatori `XOR` combinati con le potenze di 2 che definiscono la distanza del passo corrente).
// Quando due elementi devono essere confrontati e scambiati (operazione di Compare-and-Swap), non si usa un costrutto del tipo if (a > b) swap(a, b); ma invece formule matematiche dirette
// per estrarre il minimo e il massimo senza rami:
// min(a, b) = b ^ ((a ^ b) & -(a < b))
// max(a, b) = a ^ ((a ^ b) & -(a < b))
// Sfruttando il trucco del segno meno sul booleano, la CPU/GPU esegue il confronto e lo scambio in una sequenza lineare di operazioni logiche elementari, azzerando i tempi morti della pipeline.
// Complessità Temporale: O(log^2 N) sui sistemi massivamente paralleli. Su una CPU a singolo thread sarebbe un pessimo O(N \log^2 N) (più lento del MergeSort che fa O(N log N)),
// ma la sua natura rigida lo rende perfetto per l'hardware parallelo.
// GPGPU (CUDA / OpenCL):e' l'algoritmo standard utilizzato nelle GPU per ordinare array di dati massivi (come i vertici di una mesh 3D, particelle di simulazioni fisiche o indici di database grafici).
// Ogni thread della GPU si fa carico di una singola linea della rete di ordinamento, eseguendo i confronti in simultanea con miliardi di altri elementi.
// Sistemi Hard Real-Time: poiché il numero di confronti è fisso e dipende esclusivamente da N (e mai dal contenuto dell'array), il tempo di esecuzione è deterministico al microsecondo. Non esiste un "caso peggiore" che possa rallentare il sistema.

void bitonic_sort(int *arr, int n)
{
    // LOOP ESTERNO: definisce la dimensione del blocco bitonico da costruire.
    // Lavora bottom-up partendo da blocchi di 2 elementi, raddoppiando a ogni step fino a n.
    for (int block = 2; block <= n; block <<= 1)
    {
        // LOOP INTERMEDIO: definisce la distanza (passo) tra gli elementi da confrontare.
        // Esegue l'effetto "imbuto": parte dalla metà del blocco corrente (k >> 1) e si dimezza fino a 1.
        for (int pass = block >> 1; pass > 0; pass >>= 1)
        {
            // LOOP INTERNO :scorre l'intero array da 0 a n-1.
            for (int i = 0; i < n; i++)
            {
                // Calcoliamo il partner di confronto usando lo XOR bitwise.
                // pass e' una potenza di 2,quindi ha necessariamente un solo bit acceso
                // facendo lo xor con i stiamo sostanzialmente prendendo l'indice i e invertendo l'unico bit che si trova nella posizione passj
                // Se l'i-esimo elemento ha il bit in posizione pass a 0, il suo partner passj avrà quel bit a 1.
                // Questo garantisce accoppiamenti speculari perfetti senza sovrapposizioni.
                int passj = i ^ pass;
                // Per evitare di elaborare la stessa coppia due volte
                // filtriamo l'esecuzione forzando i a essere l'indice più piccolo della coppia.
                if (passj > i)
                {
                    // Determiniamo la direzione dello swap usando il blocco corrente k.
                    // L'operazione (i & k) verifica la zona geometrica dell'array.
                    // Se il risultato è 0, la zona deve essere ordinata in senso CRESCENTE.
                    // Se il risultato è diverso da 0, la zona deve essere DECRESCENTE.
                    // in sostanza quell'and e' un confronto di periodicita'(frequenza d'onda): se block = 4 il risultato si ripetera' ciclicamente ad onda ogni 4 elementi dell'array
                    int direction = ((i & block) == 0);
                    // Estraiamo i due valori correnti dall'array
                    int a = arr[i];
                    int b = arr[passj];
                    // CONFRONTO E SWAP BRANCHLESS (MIN/MAX BITWISE)
                    // La condizione determina se dobbiamo invertire gli elementi per rispettare la direzione.
                    // Calcoliamo lo XOR di scambio tra i due elementi
                    int condition = (a < b) ^ (((a > b) ^ (a < b)) & -(direction));
                    int mask = -condition;
                    // Calcoliamo lo XOR di scambio tra i due elementi
                    int diff = (a ^ b) & mask;
                    // Applichiamo la differenza simmetrica: se mask è 0, diff è 0 e i valori restano invariati.
                    // Se mask è tutto 1, diff applica lo XOR invertendo i due elementi in memoria.
                    arr[i] = a ^ diff;
                    arr[passj] = b ^ diff;
                }
            }
        }
    }
}

int main()
{
    // Test con array di 16 elementi (potenza di 2 perfetta) completamente disordinato
    int arr[16] = {14, 3, 10, 1, 16, 6, 8, 4, 12, 11, 5, 2, 15, 9, 13, 7};
    int n = 16;

    bitonic_sort(arr, n);
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n"); // Output: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16

    return 0;
}
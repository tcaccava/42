#include <stdlib.h>
#include <stdio.h>
// La funzione conta il numero di inversioni presenti in un array (dover per inversione si intende ciascuna coppia di indicii,j dove i < j ma arr[i]>arr[j])
// usando operazioni bitwise per il confronto branchless.
// Un array ordinato ha 0 inversioni, uno invertito ha n*(n-1)/2. Usato per misurare quanto un array è disordinato trova applicazione diretta in algoritmi di ranking e recommendation systems.
// Una versione semplice ma inefficiente può essere banalmente costruita con due cicli for annidati.
// int count_inversions(int *arr, int n)
// {
//     if (n < 2 || !arr)
//         return 0;
//     int inv = 0;
//     int i = 0;
//     for (; i < n - 1; i++)
//         for (int j = i + 1; j < n; j++)
//             inv += (arr[i] > arr[j]);
//     return inv;
// }
// L'espressione inv += (arr[i] > arr[j]) è un eccellente esempio di ottimizzazione branchless: invece di usare un if, sfrutta il
// fatto che in C il risultato di un confronto booleano è aritmeticamente 1 (se vero) o 0 (se falso). Il compilatore traduce questa
// riga in un'istruzione di sottomissione condizionale o di incremento diretto, evitando i micidiali branch mispredictions (errori di predizione del salto)
// della CPU, che svuoterebbero la pipeline rallentando l'esecuzione.
// Tuttavia, c'è un enorme compromesso sul piano della complessità computazionale:matematicamente una inversione misura la distanza di una permutazione dalla permutazione ordinata (identità).
// Per un array di dimensione n, il numero di confronti effettuati è n * (n - 1)/ 2,il che mappa una complessità temporale pari a O(n^2).
// Se l'array ha 10 elementi, fa circa 45 confronti (istantaneo);se ne ha 100.000 elementi, dovrà fare circa 5 miliardi di confronti, piantando la CPU per diversi secondi.
// Per gestire grandi moli di dati, la teoria degli algoritmi prevede un approccio di tipo Divide et Impera basato su una modifica del Merge Sort.
// Durante la fase di fusione (merge) di due sotto-array ordinati (sinistro e destro), se un elemento del sotto-array destro è più piccolo di un elemento del sotto-array sinistro,
// significa che quell'elemento è più piccolo di tutti i restanti elementi ancora presenti nel sotto-array sinistro. Questo permette di contare blocchi di inversioni multipli
// in un colpo solo, abbattendo la complessità a O(n log n).
// Il conteggio delle inversioni non è un mero esercizio accademico; è la metrica fondamentale dietro l'analisi delle preferenze umane e dei sistemi distribuiti:
// -sistemi di raccomandazione (Collaborative Filtering): se vuoi misurare quanto i gusti cinematografici dell'utente A siano simili a quelli dell'utente B, fai stilare a entrambi una
// classifica di 100 film. Se prendi la classifica di A e la riordini secondo l'ordine di B, il numero di inversioni calcolato ti dice matematicamente quanto i loro gusti siano distanti.
// Più il numero di inversioni è vicino a 0, più la raccomandazione sarà accurata (metrica di distanza di Kendall-Tau).
// -algoritmi di ranking (motori di ricerca): utilizzato per confrontare l'efficacia di due diversi algoritmi di indicizzazione. Se l'algoritmo A mette un sito in posizione 2
// e l'algoritmo B lo mette in posizione 10, il conteggio delle inversioni tra le due liste determina lo scostamento qualitativo dei risultati.
// -Bioinformatica: viene usato per misurare la distanza genetica tra due sequenze di DNA (allineamento dei geni), quantificando quante mutazioni o spostamenti cromosomici sono avvenuti nel corso dell'evoluzione.
// Il calcolo delle inversioni in hardware puro cambia radicalmente a seconda della scala del problema: anche senza scrivere la versione O(n log n) le CPU moderne possono accelerare drasticamente la versione O(n^2)
// grazie ai registri vettoriali (AVX2 / AVX-512 su x86, NEON su ARM). L'hardware non confronta un elemento alla volta: un intero blocco di arr[j] viene caricato in un registro a 256 o 512 bit.
// Un'unica istruzione di confronto vettoriale hardware (es. _mm256_cmpgt_epi32 su Intel) confronta arr[i] contro 8 o 16 elementi contemporaneamente in un solo ciclo di clock, generando una maschera di bit che viene poi
// accumulata tramite istruzioni di popcount (_mm256_popcnt_u32).
// In sistemi hardware dedicati in tempo reale (FPGA/ASIC per l'elaborazione dei segnali radar o lo screening di pacchetti di rete ad altissima velocità), il calcolo viene implementato tramite strutture cablate fisse
// chiamate Sorting Networks (Reti di ordinamento, come la rete di Batcher).Il circuito è composto da una griglia parallela di moduli hardware chiamati CAS (Compare-And-Swap).
// Ogni blocco CAS è una manciata di porte logiche che riceve due numeri, li confronta tramite un comparatore hardware a singolo ciclo e, se rileva un'inversione, incrementa un contatore hardware (un flip-flop accumulatore)
// mentre scambia i dati per instradarli allo stadio successivo. Questo approccio consente di calcolare le inversioni su flussi continui di dati in modalità completamente pipeline, sputando il risultato ad ogni ciclo di clock con latenza deterministica.
// L'unico svantaggio relativo della versione Divide et Impera è che mentre il codice brute-force con due cicli for annidati è efficientissimo dal punto di vista dello spazio perché lavora in-place (usa la memoria già allocata per l'array, complessità spaziale O(1)),
// il metodo ottimizzato richiede la creazione di array temporanei di supporto durante la fase di merge, comportando un costo di memoria aggiuntivo pari a O(n). Nello sviluppo software moderno, questo
// compromesso (scambiare un po' di RAM in cambio di una velocità infinitamente superiore) è la norma.
void inversion(int *half1, int *half2, int a1, int a2, int *count, int *array)
{
    int p = 0; // indice per il sottogruppo sinistro
    int q = 0; // indice destro
    int i = 0;

    for (; i < (a1 + a2); i++) // ricomponiamo i due sottogruppi ripercorrendone l'intera lunghezza
    {
        if (half1[p] <= half2[q]) // il numero di sx è inferiore a destra
        {
            array[i] = half1[p]; // lo salviamo nell'array
            p++;                 // avanziamo l'indice di sinistra
            if (p == a1)         // se siamo arrivati alla fine del sottogruppo sinistro e siamo ancora inferiori a sinistra,significa che ogni il numero sx sarà inferiore ad ogni altro numero dx,quindi esaurito il gruppo sinistro possiamo svuotare quello destro
            {
                // avanziamo la i alla posizione successiva e svuotiamo il sottogruppo destro nell'array
                i++;
                for (; q < a2; q++)
                    array[i++] = half2[q];
                break;
            }
        }
        else // il numero sinistro è superiore al destro
        {
            array[i] = half2[q]; // salviamo il numero dx nell'array
            *count += a1 - p;    // se il numero sinistro è > al dx,e i due array sono ordinati,significa che anche tutti i numeri successivi a sinistra sono superiori al numero destro
            // quindi abbiamo un numero di inversioni pari alla lunghezza del sottoarray sinistro meno l'indice a cui siamo arrivati
            q++;         // avanziamo l'indice di destra
            if (q == a2) // se abbiamo raggiunto la fine del sottogruppo destro e siamo ancora superiori a sinistra,significa che i numeri di destra sono inferiori a tutti i restanti numeri di sinistra,quindi esaurito il gruppo destro dobbiamo svuotare il sinistro
            {
                // avanziamo la i alla posizione successiva e svuotiamo half1
                i++;
                for (; p < a1; p++)
                    array[i++] = half1[p];
                break;
            }
        }
    }
}

void divide(int *array, int n, int *count)
{
    if (n == 1) // condizione base,ritorniamo quando l'array è composto da un solo elemento,che è per definizione ordinato in se stesso
        return;
    int a1 = n / 2;        // lunghezza del sottogruppo sinistro
    int a2 = a1 + (n & 1); // lunghezza di quello destro,superiore di una unità a quello sx in caso di disparità della array size
    int half1[a1];
    int half2[a2];
    int i = 0;
    for (; i < a1; i++) // copiamo la prima metà dell'array nel sottogruppo sinistro
        half1[i] = array[i];
    for (int j = 0; j < a2; j++) // la seconda metà in quello destro
        half2[j] = array[i++];
    divide(half1, a1, count); // ricorsione su entrambi i sottogruppi
    divide(half2, a2, count);
    inversion(half1, half2, a1, a2, count, array); // quando risaliamo nella call stack inversion agirà modificando,tramite il puntatore,l'array della call stack figlia,
    // ma l'array della call stack figlia è half1 o half2 della call stack padre,quindi il figlio sta fisicamente scrivendo in uno degli half del padre. Tramite i puntatori si riesce
    // quindi a garantire la persistenza anche attraverso l'intera call stack ricorsiva
}
// Anche questa versione,per quanto estremamente elegante e performante,ha un' problema: nello sviluppo di sistemi software ad alte prestazioni o firmware embedded, l'allocazione di array dinamici
// o Variable-Length Arrays (VLA) nello stack (come int half1[a1]) dentro funzioni ricorsive è severamente vietata (ad esempio negli standard automotive MISRA C).
// Se l'array iniziale avesse 100.000 elementi, la ricorsione creerebbe migliaia di sotto-array annidati nello stack, esaurendo istantaneamente la memoria e mandando il programma in crash(stack overflow).
// Nei chip dedicati o nelle implementazioni C industriali, si alloca un unico array di supporto (chiamato scratchpad o temp) nel main o nella memoria heap statica, della stessa identica dimensione dell'array originale.
// Le funzioni non creano mai nuovi array; si limitano a passarsi gli indici di inizio e fine (left, mid, right) per lavorare sempre e solo su quell'unico blocco di memoria condiviso.
// Questo garantisce che ogni modifica all'ordinamento sia immediatamente visibile a tutti i livelli della ricorsione.
// Di seguito una versione state of art per uso industriale con array di milioni di numeri.

// funzione che fonde i sotto-array e conta le inversioni
// long long merge_and_count(int *arr, int *temp, int left, int mid, int right) {
//     int i = left;      // indice di scorrimento metà sinistra
//     int j = mid + 1;   // indice di scorrimento metà destra
//     int k = left;      // indice di scrittura nell'array temporaneo
//     long long inv_count = 0;

//     // confronta e fonde in modo stabile
//     while ((i <= mid) && (j <= right)) {
//         if (arr[i] <= arr[j]) {
//             temp[k++] = arr[i++];
//         } else {
//             temp[k++] = arr[j++];
//             // se il numero a sinistra è maggiore, fa inversione con TUTTI i rimanenti a sinistra
//             inv_count += (mid - i + 1);
//         }
//     }

//     // svuota i residui della metà sinistra (se ce ne sono)
//     while (i <= mid) {
//         temp[k++] = arr[i++];
//     }

//     // svuota i residui della metà destra (se ce ne sono)
//     while (j <= right) {
//         temp[k++] = arr[j++];
//     }

//     // sicopia i dati ordinati da temp all'array nativo
//     for (i = left; i <= right; i++) {
//         arr[i] = temp[i];
//     }

//     return inv_count;
// }

// funzione ricorsiva che spacca l'array tramite gli indici
// long long merge_sort_and_count(int *arr, int *temp, int left, int right) {
//     long long inv_count = 0;

//     if (left < right) {
//         int mid = left + (right - left) / 2; // evita l'overflow matematico di (left+right)/2

//         // conta scendendo a sinistra
//         inv_count += merge_sort_and_count(arr, temp, left, mid);

//         // conta scendendo a destra
//         inv_count += merge_sort_and_count(arr, temp, mid + 1, right);

//         // conta fondendo i due blocchi
//         inv_count += merge_and_count(arr, temp, left, mid, right);
//     }
//     return inv_count;
// }

// funzione pubblica che maschera la complessità all'utente
// long long count_inversions(int *arr, int n) {
//     if (n < 2 || arr == NULL)
//         return 0;

//     // alloca l'array di supporto UNA sola volta per tutto l'algoritmo
//     int *temp = (int *)malloc(n * sizeof(int));
//     if (temp == NULL) {
//         fprintf(stderr, "Errore: Memoria insufficiente.\n");
//         exit(EXIT_FAILURE);
//     }

//     long long result = merge_sort_and_count(arr, temp, 0, n - 1);

//     // libera la memoria a lavoro finito
//     free(temp);

//     return result;
// }

// int main() {
//     int arr[] = {1, 4, 6, 5, 7, 8, 10, 9, 3};
//     int n = sizeof(arr) / sizeof(arr[0]);

//     long long inversions = count_inversions(arr, n);

//     printf("Inversioni totali: %lld\n", inversions); // Stampa: 9
//     return 0;
// }

int main()
{
    int arr[] = {1, 4, 6, 5, 7, 8, 10, 9, 3};
    int count = 0;
    divide(arr, 9, &count);
    printf("%d\n", count); // 9
    return 0;
}
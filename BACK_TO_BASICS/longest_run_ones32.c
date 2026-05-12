#include <stdio.h>
// Usiamo una versione evoluta e rivista dell'algoritmo di Kernighan usato per il popcount,nota come
// scorrimento distruttivo. Eseguendo un AND bitwise tra il numero e se stesso shiftato di una posizione
// ogni sequenza di bit accesi presente nel numero si riduce contemporaneamente di una unita',il bit di confine,indipendentemente dalla sua lunghezza
// e dalla sua collocazione binaria. Il numero di volte in cui si riesce ad eseguire questa operazione prima che il numero
// si azzeri e' essa stessa la lunghezza della sequenza di 1 piu' lunga.Perche' funziona? Perche' fare l'AND tra un numero e se stesso shiftato
// significa confrontare ogni bit col suo vicino di destra,e se quel vicino e' 0 il bit non sopravvive alla operazione. In una catena di n bit accesi,l'ultimo bit a destra
// avra' sempre uno zero alla sua destra,o il bordo del registro,quindi la coda incontrera' sempre uno zero e verra' cancellata dall'AND.
// La direzione dello shift e' irrilevante,funzionerebbe allo stesso modo sia con >> che <<,con l'unica differenza che lo scorrimento distruttivo
// inizi in testa e non in coda alla sequenza. Quindi,mentre il tradizionale Kernighan rimuove 1 bit alla volta
// dal lato LSB(per cui per es. con 30 uno sparsi fara' 30 iterazioni), questo algoritmo rimuove 1 bit da ogni gruppo ad ogni iterazione(per cui
// ad esempio con 30 uno sparsi in 10 gruppi da 3,bastano 3 iterazioni). Si tratta quindi di un algoritmo adattivo.
// Trova applicazioni reali nel Run-Length Encoding(RLE),dove serve per capire di quanto si puo' comprimere un flusso di dati
// (piu' lunghe le sequenze di 1,maggiore sara' la compressione),in analisi di immagini(Bitmaps),dove serve ad individuare la larghezza
// massima di un oggetto in una scansione monocromatica(ad esempio riconoscere le barre dei barcode) e nella gestione della memoria,per trovare il
// blocco contiguo piu' grande di pagine di memoria libere in un bitmap di allocazione.
// Dichiarare n come unsigned int non e' solo una scelta di stile,ma anche di sicurezza,perche' se fosse un signed e fosse negativo,quindi con MSB acceso,
// lo shift a destra in C sarebbe di tipo aritmetico,ossia la cpu tira dentro degli 1 da sinistra per preservare il segno,e il while n potrebbe non finire mai
// o produrre risultati imprevedibili,perche' la testa della sequenza verrebbe continuamente alimentata da 1 che entrano da sinistra. L'uso di unsigned garantisce
// che lo shift sia logico,non aritmetico,inserendo sempre nuovi zeri.
unsigned int longest_run_ones32(unsigned int n)
{
    unsigned int count = 0;
    while (n)
    {
        n &= n >> 1;
        count++;
    }
    return count;
}

int main()
{
    printf("%u %u\n", longest_run_ones32(0xFFF0FF0F), longest_run_ones32(0xFFFF0FFF)); // 12 16
}
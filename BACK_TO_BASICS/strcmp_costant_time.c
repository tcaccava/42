#include <stdio.h>

// La funzione strcmp tradizionale confronta i caratteri uno a uno,o al massimo al colpi di 8
// nelle implementazioni SWAR,e si ferma immediatamente quando trova una differenza calcolando
// proprio il gap lessicografico tra i primi due caratteri che differiscono.
// Se la password e' "password" e un attaccante invia aaaaaa,la comparazione si ferma al primo carattere
// ;se l'attaccante invia paaaaa si ferma al secondo,e cosi' via: un comportamento efficiente in termini
// prestazionali ma disastroso in termini di it security. L'attaccante potrebbe inatti misurare con estrema
// precisione il tempo che il server impiega per risponsdere,cosi' che ,per esempio,se il server impiega
// 2ms per la stringa che inizia con a e 3 per quella che inizia con p,puo' dedurne che p e' la prima lettera
// pur senza aver violato i sistemi o esfiltrato alcun dato. Con un attacco brute force e miliardi di tentativi
// la password puo' essere violata in pochi minuti in quello che e' il prototipo degli attacchi Side-Channel-Attack.
// L'obiettivo di questa funzione e' eliminare la correlazione deterministica tra il contenuto dei dati e il tempo
// di esecuzione,progettandola in modo che vengano esaminate sempre tutte le n posizioni,indipendentemente dal fatto
// che una differenza sia trovata all'inizio o alla fine,e che non ci siano salti condizionali(of,break,return anticipati)
// basati sul valore dei dati,perche' le cpu moderne usano la branch prediction che puo' variare i tempi di/
// esecuzione in base a quanto correttamente riesce a predire match invece che miss.
// In questo contesto va completamente abolita la logica booleana classia ,che e' short circuiting,cioe' pigra,
// a vantaggio della logica bitwise, sulla base della ratio che ogni singola differenza riscontrata durante il ciclo
// deve sporcare un indicatore di stato in modo irreversibile,ma senza interrompere il flusso di calcolo.Quell'indicatore,
// soltanto alla fine rivelera' se c'e' stata almeno una differenza in qualunque punto del percorso.

// PERCHÉ VOLATILE?
// Senza 'volatile', un compilatore moderno (GCC -O3) vede che 'diff' viene aggiornato
// in un loop ma non influenzerebbe il flusso del programma se uscissimo prima.
// Il compilatore potrebbe quindi inserire un "early exit" (un break implicito)
// non appena trova una differenza, per ottimizzare la velocità.
// Questo renderebbe la funzione vulnerabile a "Timing Attacks": un attaccante
// potrebbe misurare quanto tempo ci mette la funzione a rispondere e capire
// 'quanti' caratteri della password ha indovinato.

//  COS'È PROFONDAMENTE 'VOLATILE'?
//  In C, 'volatile' è un qualificatore di tipo che funge da ORDINE PERENTORIO per il compilatore.
//  Normalmente, il compilatore è un ottimizzatore ossessivo: il suo unico scopo è rendere il
//  codice più veloce. Se vede che leggi una variabile due volte, assume che il valore non sia
//  cambiato e usa una copia "veloce" nei registri della CPU (caching).
//  Dichiarare qualcosa 'volatile' significa dire: "Non fidarti della tua memoria locale".
//  Obbliga la CPU a fare un viaggio fisico sulla RAM per ogni singola operazione di lettura
//  o scrittura, senza eccezioni.
//  COSA COMPORTA LA SUA PRESENZA? (I TRE PILASTRI):
//  1) NO CACHING NEI REGISTRI: Il valore non viene mai parcheggiato temporaneamente nella CPU.
//     Ogni accesso attraversa il bus di memoria.
//  2) NO INSTRUCTION REORDERING: Il compilatore non può spostare le righe di codice per guadagnare
//     cicli di clock. L'ordine di esecuzione deve essere esattamente quello scritto.
//  3) NO DEAD STORE ELIMINATION: Se scrivi uno zero in una cella di memoria, il compilatore
//     DEVE eseguirlo, anche se la variabile non viene più usata dopo. Fondamentale per
//     cancellare password o chiavi crittografiche (zeroing memory).
//  AMBITI DI UTILIZZO:
//  - CRITTOGRAFIA (Sicurezza): Impedisce che il compilatore "tagli corto" nei confronti (early exit),
//    garantendo che il tempo di risposta sia costante (Constant Time) indipendentemente dai dati.
//  - SISTEMI EMBEDDED / HARDWARE: Per mappare registri che cambiano per eventi esterni : nei sistemi embedded
//    il sensore (o una periferica come un timer o una porta seriale) è spesso mappato direttamente in memoria RAM.
//    Questo si chiama Memory-Mapped I/O. Se per esempio un sensore scrive 0 all'indirizzo di memoria 0x4000 da cui
//    il programma legge lo stato del sensore,se nel resto del programma quel valore a quell' indirizzo non viene
//    reimpostato,il compilatore puo' decidere di non rileggere piu' quel valore in RAM, ma di salvare 0 in un registro
//    dedicato della cpu e usare sempre quello. Se,nel frattempo che la cpu esegue i suoi calcoli basandosi sullo zero
//    del registro, un evento esterno(una persona che passa davanti ad un sensore di prossimita') attiva un circuito elettrico
//    che scrive fisicamente 1 nella cella di memoria all'indirizzo 0x4000. Senza volatile la cpu continua a consultare
//    il registro interno e il sensore rimane cieco,mentre con volatile la cpu e' costretta a ignorare i registri e consultare
//    la ram attivando il bus dati,dove trovera' l'1 appena scritto dal sensore.
//  - SIGNAL HANDLERS / INTERRUPTS: Per variabili modificate da funzioni che scattano
//    improvvisamente (come un segnale di sistema SIGINT).
//  IL CASO DELLA 'STRCMP_CONSTANT_TIME':
//  In questa funzione, usiamo 'volatile' sui puntatori. Perché? Perché se il compilatore capisce
//  che stiamo cercando una differenza, la sua natura lo spingerebbe a inserire un "break" non
//  appena la trova per finire prima. Questo creerebbe un "Timing Attack": un attaccante potrebbe
//  misurare i microsecondi di differenza per capire quale carattere della password è corretto.
//  'volatile' castra questa ottimizzazione, forzando un loop lento, costante e sicuro.

int strcmp_constant_time(const unsigned char *a, const unsigned char *b, size_t n)
{
    // Castiamo a puntatori volatile: forziamo il compilatore a leggere
    // fisicamente dalla RAM ogni byte, a ogni iterazione, senza scorciatoie.
    volatile const unsigned char *va = (volatile const unsigned char *)a;
    volatile const unsigned char *vb = (volatile const unsigned char *)b;

    int diff = 0;

    while (n--)
    {
        // L'operazione XOR (^) trova la differenza tra i bit.
        // L'operatore |= (OR assegnamento) accumula qualsiasi differenza trovata.
        // Poiché va e vb sono volatili, il compilatore DEVE eseguire questo XOR
        // per n volte, garantendo un tempo di esecuzione identico (costante).
        diff |= (*va++ ^ *vb++);
    }

    // Se diff è rimasto 0, le stringhe sono identiche.
    // Ritorna 1 se sono diverse, 0 se sono uguali.
    return (diff != 0);
}
int main()
{
    unsigned char s1[] = "Hello";
    unsigned char s2[] = "Hello";
    unsigned char s3[] = "hello";
    printf("%d %d %d\n", strcmp_constant_time(s1, s2, 5), strcmp_constant_time(s1, s3, 5), strcmp_constant_time(s2, s3, 5));
}
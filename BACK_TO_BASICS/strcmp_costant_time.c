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

int strcmp_constant_time(const unsigned char *a, const unsigned char *b, size_t n)
{
    int diff = 0;
    while (n--)
        diff |= (*a++ ^ *b++); // se un solo byte differisce diff diventa,e grazie allo xor,rimane per sempre non-zero,un watermark che segnala
        // la differenza fino a fine ciclo
    return (diff != 0); 
}

int main() {
    unsigned char s1[] = "Hello";
    unsigned char s2[] = "Hello";
    unsigned char s3[] = "hello";
    printf("%d %d %d\n", strcmp_constant_time(s1, s2, 5), strcmp_constant_time(s1, s3, 5), strcmp_constant_time(s2, s3, 5));
}
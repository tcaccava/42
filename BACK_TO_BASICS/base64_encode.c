#include <stdio.h>
#include "./printbits.c"

// La codifica Base64 non serve a proteggere le informazioni,che anche codificate viaggiano in plain text,
// ma a garantire la trasmissione di dati binari grezzi(come immagini,eseguibili o chiavi crittografiche) all'interno
// di canali di comunicazione che accettano nativamente solo testo ASCII(come le email o il protocollo HTTP).
// Base64 risolve quindi un problema fondamentale,quello della incompatibilita' dei formati: i dati binari sono strutturati in
// bytes composti da 8 bit,e quindi in grado di assumere 256 valori di informazione,ma molti vecchi protocolli di comunicazione
// non sono in grado di gestire molti di quei caratteri(molti dei quali sono caratteri di controllo non stampabili,come il ritorno a capo
// o i segnali di tabulazione). Base64 risolve il problema riducendo l'alfabeto utilizzabile a soli 64 caratteri sicuri,
// ovvero tutte e 52 le lettere maiuscole e minuscole ,le dieci cifre decimali e poi + e / . Poiche'  64 e' 2 ^ 6,per rappresentare
// ognuno dei caratteri bastano esattamente 6 bit. Il nucleo matematico dell'algoritmo e' una scomposizione basata sul
// minimo comune multiplo tra la dimensione del byte di partenza (8bit) e la dimensione del carattere di destinazione(6 bit),ovvero 24 bit.
// Questa uguaglianza si traduce in una regola fissa: ogni blocco di 3 byte in input(24 bit totali) viene redistribuito esattamente in un blocco
// di 4 caratteri in output(6 bit ciascuno).
// Si possono quindi configurare 3 casi:
//  1)Abbiamo un numero di bytes multiplo di 3. Da ciascun chunk di 3 bytes B0 B1 B2 dobbiamo estrarre 4 finestre di 6 bit ciascuna(W_0, W_1, W_2, W_3)
//  W0 = primi 6 bit di B_0): si ottiene isolando i 6 bit più significativi del primo byte.
//  W1 = ultimi 2 bit di B_0 + primi 4 bit di B1): e' un ponte; prende la coda del primo byte e la unisce alla testa del secondo.
//  W2 = ultimi 4 bit di B_1 + primi 2 bit di B_2): altro ponte; prende la coda del secondo byte e la unisce alla testa del terzo.
//  W3 = ultimi 6 bit di B_2): prende gli ultimi 6 bit rimasti del terzo byte.
//  Ognuno di questi quattro indici estratti (che matematicamente andrà da 0 a 63,quindi il bit 6 e 7 saranno sempre 9) fungerà
//  da coordinata per pescare il rispettivo carattere ASCII stampabile da una tabella di traduzione fissa (Lookup Table).
//  2)Il modulo del numero di bytes su 3 e' uguale a 1,quindi avanza un byte. L'algoritmo estrae i 6 bit piu' significativi,usa i due bit rimanenti
//  per creare un nuovo chunk di 6 bit aggiungendoci 4 zeri artificiali a destra. I restanti due byte del quartetto di byte vengono forzati a =(padding)
//  3)Avanzano due bytes: si estraggono i primi due blocchi di 6 bit completi,i 4 bit rimamenti vengono completati con due zeri a destra . Solo l'ultimo
//  carattere del quartetto diventa =
//  Applicazioni Pratiche e Industriali
// Base64 è onnipresente nell'architettura del web moderno e dei sistemi informativi:
// MIME e Allegati Email (SMTP): il protocollo SMTP, che invia le email in tutto il mondo dal 1982, è nato per gestire esclusivamente testo ASCII a 7 bit.
// Quando alleghi un PDF, un'immagine JPG o un archivio ZIP a un'email, il tuo client (Outlook, Thunderbird, Gmail) trasforma istantaneamente quel file binario
// in una gigantesca stringa Base64 per farlo viaggiare in sicurezza sui server di posta senza che i router interpretino i byte binari come comandi di interruzione della trasmissione.
// Data URIs nello sviluppo Web: nell'ottimizzazione delle pagine web (SEO e performance), i designer a volte codificano piccole immagini o font direttamente all'interno del codice
// HTML o CSS usando la sintassi data:image/png;base64, .Questo azzera il numero di richieste HTTP necessarie al browser per scaricare gli asset secondari,
// velocizzando il caricamento della pagina.
// Token di Autenticazione (JWT): i JSON Web Tokens, utilizzati per gestire le sessioni di login nelle moderne API web, contengono informazioni sull'utente e firme crittografiche.
// Questi dati vengono codificati in una variante del Base64 ("URL-safe", che sostituisce + e / con - e _) per poter essere passati in sicurezza all'interno degli header HTTP o degli
// indirizzi URL senza generare conflitti con i caratteri speciali del browser.

// Implementazione Hardware: lo Splitting parallelo
// A livello di silicio, la codifica Base64 è un'operazione banale ed estremamente economica, poiché l'estrazione dei bit non richiede calcoli logici complessi o stadi sequenziali iterativi.
// In un chip hardware dedicato (ad esempio in una scheda di rete che deve codificare dati al volo, o in un coprocessore multimediale), il circuito di estrazione dei 6 bit
// viene cablato in puro hardware statico (routing).
// I registri di input accolgono i 3 byte in parallelo (24 linee elettriche parallele).Queste 24 piste di rame si sdoppiano direttamente in 4 gruppi da 6 piste ciascuno.
// Le piste che compongono i "ponti" (come il passaggio tra il byte 0 e il byte 1) vengono convogliate fisicamente agli ingressi dei registri di output senza bisogno di
// porte logiche intermedie. Il costo in termini di transistor per la scomposizione dei bit è letteralmente zero.
// La Lookup Table in Silicio: ROM o Decoder
// L'unico vero componente attivo del circuito è lo stadio di traduzione dall'indice a 6 bit al carattere ASCII a 8 bit. Questo può essere implementato in due modi nell'hardware:
// Tabella ROM statica: una piccolissima memoria a sola lettura da 64 ingressi x 8 bit. L'indice a 6 bit fa da indirizzo di memoria e la ROM sputa fuori il corrispondente carattere ASCII in un singolo ciclo di clock.
// Logica combinatoria (Decoder): un blocco di porte logiche (AND/OR/NOT) ottimizzato per sintetizzare la mappa dei caratteri. Questa soluzione è ancora più veloce della ROM perché riduce il ritardo
// di propagazione elettrica a frazioni di nanosecondo.
// Visto che i quattro blocchi da 6 bit sono indipendenti, l'hardware può calcolare tutti e 4 i caratteri ASCII di output contemporaneamente nello stesso ciclo di clock.
// La pipeline può accettare 3 nuovi byte ad ogni impulso, garantendo un throughput di codifica spaventoso, limitato solo dalla velocità con cui i dati possono essere inseriti nei registri di ingresso.
// La tabella di lookup (chiamata spesso anche alfabeto Base64) non devi inventarla o cercarla in qualche libreria nascosta del sistema operativo: è definita dallo standard ufficiale (RFC 4648).
// È una stringa fissa di esattamente 64 caratteri stampabili messi in un ordine ben preciso:0 – 25,da A a Z ,26 – 51 da a a z ,52 – 61 da 0 a 9 , 62 il carattere +, 63 il carattere /
// Base64 URL-Safe: l'algoritmo matematico e gli shift rimangono identici al 100%. L'unica cosa che cambia è la tabella di lookup alla fine: poiché i caratteri + e / hanno un significato speciale negli
// indirizzi web (URL) e rischiano di spaccare i link, la variante URL-Safe sostituisce gli ultimi due indici della stringa con - e _
void module_0(unsigned char **src, size_t *src_len, unsigned char **dst)
{
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    while (*src_len >= 3)
    {
        **dst = base64_table[(**src & 252) >> 2];
        *dst += 1;
        **dst = base64_table[(**src & 3) << 4 | ((*(*src + 1) & 0xF0) >> 4)];
        *dst += 1;
        **dst = base64_table[((*(*src + 1) & 0x0F) << 2) | ((*(*src + 2) & 192) >> 6)];
        *dst += 1;
        **dst = base64_table[*(*src + 2) & 63];
        *dst += 1;
        *src += 3;
        *src_len -= 3;
    }
}

void module_1(unsigned char **src, unsigned char **dst)
{
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    **dst = base64_table[(**src & 252) >> 2];
    *dst += 1;
    **dst = base64_table[(**src & 3) << 4];
    *dst += 1;
    **dst = 61;
    *dst += 1;
    **dst = 61;
    *dst += 1;
}

void module_2(unsigned char **src, unsigned char **dst)
{
    static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    **dst = base64_table[(**src & 252) >> 2];
    *dst += 1;
    **dst = base64_table[((**src & 3) << 4) | ((*(*src + 1) & 0xF0) >> 4)];
    *dst += 1;
    **dst = base64_table[(*(*src + 1) & 0x0F) << 2];
    *dst += 1;
    **dst = 61;
    *dst += 1;
}
void base64_encode(unsigned char *src, size_t src_len, unsigned char *dst)
{
    size_t remaining = src_len;
    // Consuma tutti i blocchi da 3 byte possibili
    module_0(&src, &remaining, &dst);
    // Gestisci i residui rimasti usando i dati aggiornati da module_0
    if (remaining == 1)
        module_1(&src, &dst);
    else if (remaining == 2)
        module_2(&src, &dst);
    *dst = '\0';
}

// void base64_encode(unsigned char *src, size_t src_len, unsigned char *dst)
// {
//     static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//     size_t i = 0;
//     size_t j = 0;

//     Ciclo principale: elabora i blocchi completi da 3 byte
//     while (i + 2 < src_len)
//     {
//         dst[j++] = b64[(src[i] >> 2) & 0x3F];
//         dst[j++] = b64[((src[i] << 4) | (src[i + 1] >> 4)) & 0x3F];
//         dst[j++] = b64[((src[i + 1] << 2) | (src[i + 2] >> 6)) & 0x3F];
//         dst[j++] = b64[src[i + 2] & 0x3F];
//         i += 3;
//     }

//     Gestione dei residui (il padding) direttamente alla fine
//     if (i < src_len)
//     {
//         Il primo carattere del blocco finale c'è sempre se avanza qualcosa
//         dst[j++] = b64[(src[i] >> 2) & 0x3F];

//         if (src_len - i == 1) // Avanza 1 solo byte (es. "M")
//         {
//             dst[j++] = b64[(src[i] << 4) & 0x3F];
//             dst[j++] = '=';
//             dst[j++] = '=';
//         }
//         else Avanzano 2 byte (es. "Ma")
//         {
//             dst[j++] = b64[((src[i] << 4) | (src[i + 1] >> 4)) & 0x3F];
//             dst[j++] = b64[(src[i + 1] << 2) & 0x3F];
//             dst[j++] = '=';
//         }
//     }

int main()
{
    unsigned char dst[5]; // 4 caratteri + 1 per il terminatore '\0'

    base64_encode((unsigned char *)"Man", 3, dst);
    printf("%s\n", dst); // TWFu

    base64_encode((unsigned char *)"M", 1, dst);
    printf("%s\n", dst); // TQ==

    base64_encode((unsigned char *)"Ma", 2, dst);
    printf("%s\n", dst); // TWE=

    return 0;
}
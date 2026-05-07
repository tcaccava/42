#include <stdio.h>
// La DJB2 e' una Non cryptographic hash function che prende il nome dalle iniziali del suo creatore di Daniel J. Bernstein.
// Usata in compilatori, dizionari, e tabelle hash. E' una funzione estremamente efficiente
// che trasforma una stringa,intesa come una sequenza di byte di lunghezza variabile,in un numero intero,il suo hash.
// Una buona funzione di hashing deve essere rapida a calcolare l' hash e deve garantire distribuzione adeguati degli hash ottenuti,
// cioe' deve spargere i risultati in modo uniforme per evitare collisini. Bernstein scopri' che iterando su una stringa e moltiplicando
// costantemente il valore accumulato per 33,si otteneva un numero minimo di collisioni. La funzione non parte da un hash di 0 ma settato
// ad un valore seed di 5381,un numero primo che,benche' manchi una dimostrazione matematica inoppugnabile,ha dimostrato
// nei migliaia di test di costanti eseguiti da Bernstein di produrre statisticamente una distribuzione migliore con set di stringhe
// reali(nomi di file,parole del dizionario). In realta' esiste in parte una base matematica: un numero dispari e primo nel ssitema binario
// ha una bassa densita' di bit, che aiuta a non saturare i bit alti troppo velocemente durante i primi calcoli.
// La base matematica del 33 come moltiplicatore invece e' piu' solida: moltiplicare per 33 equivale a shiftare di 5 << e aggiungere hash:
// lo shift di 5,rispetto ad altri valori,e' lo sweet spot che si colloca a meta' tra l'estremo di pochi bit,che porta i nuovi caratteri
// ad influenzare solo una piccola parte del numero finale,e l'estremo dello shift di troppi bit,che a causa dell'overflow porta ad una perdita
// irreversibile di informazione: la maggior parte delle stringhe piu' comuni e' fatta di caratteri ascii,e questi usano solo 7 bit. Shiftando di 5
// si crea una sovrapposizione parziale che mescola i bit dei vecchi e dei nuovi caratteri in modo caotico,che e' propio cio` che Berstein cercava.
// Quindi si puo` dire che la base matematica esista,solo e' una base statistica ed euristica,non deterministica.
// Inoltre moltiplicare hash * 33 equivale a shiftarlo di 5 volte a sinistra,e poi aggiungere hash,e quindi e' un operazione
// che si presta ad essere eseguita usando operatori bitwise che la cpu esegue instantaneamente.
// Esistono due varianti storiche di DJB2,una aggiunge il carattere al risultato della moltiplicazione,l'altra fa Xor con lo stesso. La variante originaria
// usava l'addizione,ma molte implementazioni moderne usano lo XOR.
// Questa funzione ha notevole importanza anche per la implementazione che la djb2 ha in Python: quando in questo linguaggi accedi ad una chiave per ottenere il valore
// l'interprete non scorre tutti gli elementi del dizionario per trovare la chiave,ma la trasforma in un intero usando una funzione di hash. Fino a Python 3.2
// la funzione integrata hash() era una variante modificata della logica di Bernstein.I dizionari Python,che sono a tutti gli effetti Hash Tables,usano proprio
// questo algoritmo per accedere istantaneamente al valore associato alla chiave con un accesso istantaneo O(1),indipendentemente dal numero di chiavi nel dizionario.
// Recentemente Python e' passato ad algoritmi piu' complessi come il SipHash cn seed randomico per ogni sessione,per motivi di sicurezza: un hacker potrebbe inviare migliaia di stringhe
// ingegnerizzate a tavolino per ottenere lo stesso hash,il che renderebbe il dizionario catastroficamente lento fino a poter causare il crash del server: attacco Dos.
// Per evitare questo Python usa il salting all'inizio di ogni sessione,rendendo questi attacchi impossibili.

unsigned int hash_djb2(unsigned char *str)
{
    unsigned int hash = 5381; // l'hash deve essere unsigned per una gestione pulita dell'overflow
    while (*str)
        hash = ((hash << 5) + hash) ^ *str++;
    return hash;
}

int main()
{
    unsigned char *s = "Hello";
    unsigned char *s2 = "hello";
    printf("%u %u\n", hash_djb2(s), hash_djb2(s2)); // due stringhe differenti solo per una lettera producono hash drammaticamente diversi
}
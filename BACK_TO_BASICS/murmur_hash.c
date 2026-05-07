#include <stdio.h>

// La MurmurHash3 e' l'artiglieria pesante delle funzioni di hash non crittografiche
// Murmur sta per Multiply-Rotate-Multiply-Rotate,una tecnica che porta l'effetto valanga
// all'estremo:
// 1) Moltiplicazione per costanti irrazionali: le costanti 0xcc9e2d51 e 0x1b873593 sono numeri primi
// che hanno una distribuzione di bit molto alta(molti 1 e 0 alternati). Moltiplicando un numero per queste
// costanti,i bit originali vengono spalmati su tutta la larghezza dei 32 bit,in maniera tale che ogni bit del numero
// influenzi quasi tutti i bit del risultato.
// 2) La rotazione bitwise(ROL): a differenza dello shift << che porta a perdere i bit shiftati dall'MSB,
// la rotazione (x << n) | (x >> (32-n)) prende i bit che escono da una parte e li reinserisce all' estremo
// opposto,quindi non viene persa informazione:ogni bit di entropia che si aveva all'inizio rimane nel sistema,
// ma spostato di posizione. La scelta di rotazioni di 15 e 13 bit serva ad evitare che i bit tornino troppo velocemente
// nella stessa posizione dopo poche iterazioni.
// 3) Lo XOR viene usato per iniettare il blocco dati appena lavorato nello stato globale dell'hash.
// Questo algoritmo e' usato in Redis e Hadoop,sistemi che devono gestire milioni di chiavi al secondo,e quindi che
// necessitano di uniformita' quasi perfetta(se inserisci un milione di stringhe quasi uguali in un db,murmur mix le distribuira'
// cosi' bene che ogni colonna della tabella avra quasi lo stesso numero di elementi), e resilienza,cioe' un estrema
// difficolta' nel generare collisioni,grazie alla continua alternanza tra moltiplicazioni(algebra lineare) e rotazioni
// (algebra bitwise). La combinazione di questi due mondi rende estremamente complesso revertire la loro combinazione.
// Una volta iniettato k tramite XOR, l'hash viene ruotato, moltiplicato per 5 e poi gli viene aggiunta una altra costante (0xe6546b64).
// L'operazione hash * 5 + costante e' un generatore congruenziale lineare, cioe' un algoritmo per la generazione di numeri pseudocasuali
// che serve a garantire che, nel caso in cui lo XOR abbia prodotto
// molti zeri, riporti l'hash in un range di valori elevati.
// A differenza di DJB2 e FNV-1a,MurmurHash3 non ha un seed fisso universale stabilito dall'algoritmo stesso,ma il seed viene passato come
// argomento stesso (h) della funzione principale. Questa flessibilita' e' la chiave della resistenza di questo algoritmo agli
// attacchi Hash Flooding: se un server cambia il seed dinamicamente ad ogni riavvio,un hacker non potra' mai prevedere quali stringhe causano collisioni
// perche' non puo' prevedere con certezza il valore iniziale di h.
// ALgoritmi come Murmurmix e simili sono usati da Redis Cache, un sistema di cache che indicizza i dati usando per ognuno di essi un hash calcolato
// con murmurmix o simili,che garantiscono una distribuzione adeguata degli hash in maniera tale che dati simili generino hash diversi,riducendo il rischio di collisioni,ma che
// al tempo stesso la distribuzione di questi hash sia piu o meno uniforme,cosi da fare in modo che si possa installare su server diversi e che il carico sia uniformemente distribuito tra gli stessi.
// Quindi combina l'entropia necessaria ad evitare collisioni ma non troppa follia per evitare che i dati si accumulino tutti in cluster di hash vicini e quindi prevedibili.
// In informatica questo concetto si chiama Uniformità della Distribuzione: se l'hash fosse "troppo folle" o non deterministico, perderesti i dati; se lo fosse troppo poco, creeresti dei "cluster" (accumuli).
// Il segreto e' nel bilanciamento tra Entropia e Uniformità: l'entropia serve a separare dati simili (es. "user_1" e "user_2"),l'uniformità serve a garantire che, su un range di hash da 0 a 2
// 32 −1, la probabilità che un hash cada in un punto qualsiasi sia identica. Senza uniformità: Avresti un server strapieno e uno vuoto.
// Con uniformità ogni server riceve circa il 1/N dei dati (dove N è il numero di server).
// Altro concetto fondamentale e' quello di Consistent Hashing (Sharding): quando la cache di Redis(che ricordiamo risiede in ram) cresce e non basta più un solo server,
// si passa al Redis Cluster. Invece di decidere a caso dove mettere un dato, si usa l'hash della chiave per assegnarla a uno "slot".
// Redis divide lo spazio degli hash in 16384 slot: ogni server del cluster è responsabile di un range di questi slot.
// Grazie a MurmurHash (o algoritmi simili come CRC16), la chiave viene "sparata" in uno slot preciso in modo prevedibile ma uniforme.
// Il vantaggio di questo sistema è che se aggiungi un nuovo server, non devi ricalcolare tutto da zero. Grazie alla distribuzione uniforme,
// "sposti" solo una piccola porzione di hash dal vecchio al nuovo server, mantenendo il sistema in equilibrio.
// Nonostante MurmurMix sia eccellente, statisticamente le collisioni possono avvenire(paradosso del compleanno). Redis le gestisce internamente usando solitamente
// il Chaining(o concatenazione) : se due chiavi diverse finiscono nello stesso "cassetto" della tabella hash,
// Redis crea una piccola lista collegata in quel cassetto,per cui quando cerchi la chiave, Redis calcola l'hash e poi scorre la piccola lista per trovare quella esatta.
// Se l'algoritmo di hash è valido (come Murmur), queste liste restano cortissime (spesso un solo elemento), mantenendo la velocità O(1).
unsigned int ft_murmur_mix(unsigned int h, unsigned int k)
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    h ^= k;
    h = (h << 13) | (h >> 19);
    h = h * 5 + 0xe6546b64;
    return h;
}

int main()
{
    // un solo bit di differenza,hash completamente diversi
    printf("%u\n", murmur_mix(0, 0x95)); // 1890812043
    printf("%u\n", murmur_mix(0, 0x96)); // 2972588701
}
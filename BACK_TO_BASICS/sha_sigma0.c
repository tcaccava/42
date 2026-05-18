#include <stdio.h>
#include "./ror32.c"
// La  funzione Sigma 0,nello standard ufficiale FIPS 180-4 dello SHA 256,si divide in due categorie:
// - sigma minuscolo 0,che usa ROR 7,18 e shift >> 3 ed e' il motore della Message Schedule(l'espansione del messaggio da 16 a 64 parole)
// - sigma maiuscolo 0,che usa la formula con ROR 2,13,22 ed e' il cuore pulsante della funzione di compressione interna,quella che
// elabora direttamente le variabili di stato(nello specifico agisce sul registro di lavoro A ad ogni round)
// L'obiettivo fondamentale di qualsiasi funzione di hash crittografica e' massimizzare l'avalanche effect,cioe' la capacita',modificando un singolo
// bit di input,di cambiare in modo imprevedibile circa la meata' dei bit in output,col minimo costo computazionale
// I tre valori di ROR sono il risultato di una ricerca di ottimizzazione combinatiria per massimizzare la dispersione dei bit riducendo al minimo
// le collisioni locali. Le distanze relative delle rotazioni sono 11(13 -2), 9(22 - 13) e la distanza ciclica di ritorno 12(32 - 22 + 2).
// I numeri di ror e le loro differenze relative non sono multipli diretti l'uno dell'altro nel modulo 32,e questa asimmetria garantisce che,se un pattern
// di bit alterati si presenta in input,le tre copie ruotate non si sovrappongono mai in modo da annullarsi a vicenda tramite lo xor,tranne che in casi rarissimi
// Ogni singolo bit di input viene proiettato e spalmato contemporaneamente su tre posizioni di output distinte:i - 2,i - 13,i - 22.
// Se usata da sola sigma maiuscola sarebbe debole perche' e' una funzione lineare:l'output dello xor di due numeri e' uguale allo xor degli output.
// La vera potenza crittografica emerge quando il risultato di sigma maiuscolo viene iniettato nel blocco successivo dello SHA 256,dove subisce una addizione modulare
// (somma algebrica modulo 2 ^32) con le altre variabili. L'alternanza di XOR e ROR e operazioni aritmetiche di addizione distrugge completamente la linearita'
// algebrica,rendendo il sistema impossibile da invertire tramite sistemi di equazioni lineari.
// Grazie alla rotazione circolare nessun bit morto viene espluso o person come nello shift logico e l'entropia iniziale del dato viene preservata al 100%,ma redistribuita
// nello spazio della parola a 32 bit.
// Se a livello software l'operazione richiede istruzioni specifiche della CPU per manipolare i registri, a livello hardware nativo
// (ASIC, FPGA, circuiti integrati dedicati al mining di Bitcoin o alla sicurezza nei chip d'interfaccia) la funzione Sigma_0 rivela una proprietà sbalorditiva.
// In un circuito logico hardware, una rotazione fissa a destra di N posizioni non richiede porte logiche, non consuma silicio e ha un tempo di propagazione pari a zero.
// Non esistono transistor dedicati allo spostamento.È un puro fatto di routing (instradamento dei cavi). Il progettista hardware prende fisicamente la pista di
// rame che porta il bit 0 e la collega direttamente all'ingresso della porta logica che deve calcolare il bit 2 del livello successivo. I fili vengono semplicemente incrociati sul silicio.
// Il vero e unico costo hardware della funzione è rappresentato dalle operazioni di XOR a tre vie
// Per ogni bit della parola (32 bit totali), sono necessarie porte logiche XOR in grado di gestire tre ingressi. In base alla tecnologia costruttiva, questo si traduce in una
// cella standard XOR3 o in due livelli cascanti di porte XOR standard a due ingressi (XOR2). Il consumo di porte è lineare ed estremamente ridotto (64 porte XOR2 totali per l'intera funzione a 32 bit).
// Ritardo di Propagazione: il tempo richiesto per completare l'intera funzione è pari al ritardo di transizione di un singolo blocco XOR a tre ingressi (o due passaggi di porte logiche elementari).
// Tutti i 32 bit dell'output vengono generati in parallelo nello stesso istante.Questa parallelizzazione hardware assoluta, combinata al costo zero delle rotazioni, permette ai chip di hashing
// di eseguire questa funzione a frequenze di clock elevatissime (nell'ordine dei gigahertz), rendendo il calcolo di Sigma_0 uno dei componenti più veloci e ottimizzati dell'intera architettura hardware
// di sicurezza moderna.

unsigned int sha256_sigma0(unsigned int x)
{
    unsigned int first_ror = ror32(x, 2);
    unsigned int second_ror = ror32(x, 13);
    unsigned int third_ror = ror32(x, 22);
    return first_ror ^ second_ror ^ third_ror;
}
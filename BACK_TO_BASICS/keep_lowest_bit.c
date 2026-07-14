/**
 * =====================================================================================
 * @file        keep_lowest_bit.c
 * @brief       Isolamento dell'ultimo bit acceso (LSB) tramite algebra booleana.
 *
 * 1. DEEP-DIVE TEORICO: MATEMATICA DEL COMPLEMENTO A DUE E PROPRIETÀ DEI GRUPPI
 *    In un'architettura di calcolo a n-bit, l'insieme dei valori rappresentabili forma 
 *    un anello algebrico di congruenza modulo 2^k, denotato come Z/(2^k)Z. In questo 
 *    spazio, la sottrazione non esiste come entità primaria, ma è definita tramite 
 *    l'elemento inverso additivo. Per ogni intero n, il suo inverso -n è quell'elemento 
 *    tale per cui: n + (-n) = 0 (mod 2^k).
 *
 *    La rappresentazione in complemento a due mappa questa proprietà a livello hardware. 
 *    Sia n un intero espresso in base 2. Possiamo isolare la sua componente inferiore 
 *    strutturandola come n = X·2^m, dove 2^m rappresenta la minima potenza di due che 
 *    divide n (ovvero, il bit m-esimo è il Least Significant Bit impostato a 1, mentre 
 *    tutti i bit da 0 a m-1 sono uguali a 0). La stringa binaria di n si presenta come:
 *    n = [stringa_superiore_X] 1 [000...0_stringa_inferiore]
 *
 *    Applicando l'operatore di inversione bitwise NOT (~n), invertiamo l'intera topologia 
 *    del registro:
 *    ~n = [~stringa_superiore_X] 0 [111...1_stringa_inferiore]
 *
 *    Quando eseguiamo l'operazione aritmetica di incremento (+1) su ~n per ottenere il 
 *    negativo hardware (-n = ~n + 1), scateniamo una catena di riporto (carry chain) 
 *    nella ALU. Il carry entra nella posizione 0, che contiene un 1, e propaga verso 
 *    sinistra commutando tutti gli '1' della stringa inferiore in '0'. La propagazione 
 *    si arresta tassativamente al primo '0' disponibile, che corrisponde esattamente 
 *    alla posizione m (l'LSB originale di n). Questo '0' commuta a '1', mentre la 
 *    stringa superiore (~X) rimane completamente inalterata:
 *    -n = [~stringa_superiore_X] 1 [000...0_stringa_inferiore]
 *
 *    Eseguendo infine l'operazione logica di AND bitwise (n & -n), le due stringhe 
 *    vengono intersecate:
 *    n  = [ X] 1 [0...0]
 *    -n = [~X] 1 [0...0]
 *    -------------------
 *    &  = [ 0] 1 [0...0]  -> Poiché X & ~X = 0 per l'assioma di non-contraddizione.
 *
 *    Il risultato è l'isolamento matematico perfetto del valore puramente geometrico 
 *    dell'LSB (2^m), ottenuto in tempo costante.
 *
 * 2. APPLICAZIONI PRATICHE E SISTEMISTICHE DI LIVELLO INDUSTRIALE:
 *    - Alberi di Fenwick (Binary Indexed Trees - BIT): Questa struttura dati avanzata 
 *      utilizza l'operazione 'idx & (-idx)' come funzione di transizione fondamentale per 
 *      risolvere query di somma su range e aggiornamenti puntuali in tempo O(log N).
 *    - Motori Scacchistici (Bitboards): La rappresentazione della scacchiera avviene tramite 
 *      interi a 64-bit (uint64_t). L'isolamento e la successiva rimozione dell'LSB 
 *      permettono di scansionare le posizioni dei pezzi legali sulla scacchiera alla 
 *      velocità massima consentita dal silicio.
 *    - Allocatori di memoria a bassa latenza: Scansione istantanea di bitmap di allocazione 
 *      per determinare l'indice del primo blocco di memoria libero disponibile.
 *
 * 3. IMPLEMENTAZIONE HARDWARE E EFFICIENZA NELLA PIPELINE:
 *    A livello di codice macchina, l'espressione 'n & (-n)' viene tradotta da qualsiasi 
 *    compilatore moderno in due sole istruzioni assembly sequenziali. Ad esempio su x86_64:
 *      NEG eax    ; Calcola il complemento a due (inverte i bit e aggiunge 1)
 *      AND eax, edi ; Esegue l'AND logico con il valore originale
 *
 *    Queste istruzioni vengono elaborate direttamente all'interno delle unità di esecuzione 
 *    intere della ALU. Entrambe le istruzioni hanno una latenza di esattamente 1 ciclo di 
 *    clock sulla stragrande maggioranza delle microarchitetture (Intel Core, AMD Zen, ARM Cortex).
 *    Essendo un algoritmo intrinsecamente branchless, non introduce alcuna istruzione di salto 
 *    condizionale. Di conseguenza, il rischio di branch misprediction è pari a zero, consentendo 
 *    alla pipeline di esecuzione della CPU di operare a saturazione senza subire penalità di 
 *    stallo (pipeline flush).
 * =====================================================================================
 */

#include <stdio.h>

unsigned char keep_lowest_bit(unsigned char n) {
    return n &(-n);
}

//il trucco sta nel complemento a due: un numero binario non puo' essere negativo,quindi
//si stabilisce che il negativo di n e' il numero che ,sommato ad n,da 0.
//per un numero a 8 bit per esempio il negativo di n e' quel numero che
//sommato ad n da 10000000,quindi un numero a 9 bit,di cui viene troncato il MSB,quindi si ottiene 
//00000000,ovvero 0. Per esempio il negativo di 00001001,cioe 9,e' quel numero che sommatogli da
//100000000,che e' 256,e quel numero e' quindi 247,cioe 11110111.Quindi -9 a 8 bit e' 11110111.
//Ovviamente per vederlo stampato da printf come -9 bisogna passargli un tipo signed come char,non unsigned char
//Il complemento a due consiste quindi.per passare dal positivo al negativo,nell' invertire tutti i bit e 
//poi aggiungere uno. Il fondamento sta nel fatto che sommando un numero al suo inverso in bit,ottenibile con la ~,
//si ottiene un binario con tutti 1,e per ottenere l'overflow di cui sopra bisogna ancora aggiungere 1.
//Quindi il negativo di un numero e' il suo inverso in bit + 1;
//Ma come arriviamo da questo a keep_lowest_bit? Supponiamo di avere 10110100,cioe 180. Il suo negativo -n e'
//n invertito + 1,quindi ~n + 1,cioe 01001011 + 1,cioe 75 + 1,cioe 76.Infatti 180 + 76 = 256,overflow a 8 bit.
//Quando aggiungo 1 ad 01001011 succede che il carry si propaga fino da destra fino a che non trova uno zero,
//e quel primo zero diventa 1,cioe' 01001100. Questo numero ha tutti i bit sopra il primo 1 di n invertiti,
//e tutti i bit sotto azzerati. L'unico elemento immodificato e' il primo 1 di n,e quindi l'AND
//tra n e il suo negativo isola il primo bit acceso in n.

int main() {
    char meno_nove = 0b11110111;
    printf("%d\n",meno_nove);// -9
    char complemento_a_due = ~
    printf("%d\n", keep_lowest_bit(0b00101001));// 00000001
    printf("%d\n", keep_lowest_bit(0b00010000));// 00010000
}
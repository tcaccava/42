/*
 * =====================================================================================
 * CARRY_SAVE_ADD3.c - CARRY-SAVE ADDER (CSA)
 * =====================================================================================
 * 
 * 1. IL CONCETTO (IL "PERCHÉ"):
 * In un addizionatore classico (Ripple Carry Adder), per sommare due numeri, il riporto 
 * (carry) del bit 0 deve propagarsi al bit 1, poi al 2, e così via fino alla fine. 
 * È un processo seriale: la velocità è limitata dalla propagazione fisica del segnale.
 * 
 * Il Carry-Save Adder (CSA) rompe questo vincolo. Invece di propagare il carry, lo 
 * "salva" (da qui Carry-Save). Prende 3 ingressi (A, B, C) e sputa fuori 2 uscite:
 * - Una 'Sum': La somma parziale senza considerare i riporti.
 * - Un 'Carry': I riporti generati, pronti per essere sommati al livello successivo.
 * 
 * 2. LA MATEMATICA DEL CSA (COME FUNZIONA BIT A BIT):
 * - Prende 3 bit dalla stessa colonna spaziale (A, B, C):
 * - Sum = a ^ b ^ c
 *   Usa l'XOR per calcolare la somma isolata (senza riporti). 
 * - Carry = ((a & b) | (b & c) | (a & c)) << 1
 *   Usa la funzione di maggioranza per capire se si è generato un riporto (almeno 
 *   due bit a 1). Il "<< 1" sposta il riporto nella colonna di sinistra, esattamente 
 *   dove dovrà essere sommato al ciclo successivo.
 *   
 * 3. IL VERO SCOPO: LA MOLTIPLICAZIONE COME "MASS-ADDITION"
 * Il Carry-Save Adder (CSA) non viene usato per fare "A + B". Viene usato nei 
 * moltiplicatori hardware per sommare decine di "prodotti parziali" simultaneamente.
 * La moltiplicazione binaria non è altro che una gigantesca addizione di molte righe.
 * 
 * 4. COME SI OTTENGONO I PRODOTTI PARZIALI (LE RIGHE):
 * Immagina di moltiplicare due numeri a 32 bit, Moltiplicando (M) x Moltiplicatore (Q).
 * L'hardware analizza i bit di Q uno ad uno:
 * - Se il bit 0 di Q è 1, la prima riga è M. Se è 0, la prima riga è 0.
 * - Se il bit 1 di Q è 1, la seconda riga è M shiftato a sinistra di 1. Altrimenti 0.
 * - E così via per 32 volte.
 * Risultato: ti ritrovi con 32 righe di numeri binari che devono essere sommate.
 * 
 * 5. IL PROBLEMA DELL'ADDIZIONE CLASSICA:
 * Sommare 32 righe con un addizionatore standard (Ripple Carry Adder) significa 
 * sommare riga 1 + riga 2, aspettare che il riporto arrivi alla fine, poi sommare 
 * la riga 3, aspettare il riporto, ecc. È un processo seriale lentissimo (O(N)).
 * 
 * 6. LA SOLUZIONE CSA: IL COMPRESSORE 3-A-2 (WALLACE TREE):
 * Il CSA prende 3 righe in ingresso e le comprime in 2 righe in uscita in tempo O(1),
 * SENZA ASPETTARE LA PROPAGAZIONE DEL RIPORTO.
 * - Invece di sommare tutto insieme, costruisci un albero (Wallace Tree):
 * - Prendi le tue 32 righe a gruppi di 3 e le dai in pasto a vari CSA in parallelo.
 * - Le 32 righe diventano ~22 righe.
 * - Le 22 diventano ~15.
 * - Le 15 diventano ~10... e così via (crescita logaritmica O(log N)).
 * 
 * Alla fine dell'albero, rimangono SOLO 2 RIGHE. 
 * A questo punto (e solo a questo punto), si usa un addizionatore classico e veloce
 * (Carry-Lookahead) per sommare le ultime due righe e ottenere il prodotto finale.
 * 
 * 
 * 7. APPLICAZIONI PRATICHE:
 * - Wallace Tree / Dadda Tree: Per sommare N numeri (es. 64 o 128 bit in crittografia), 
 *   non sommi uno dopo l'altro. Usi un albero di CSA. Questo riduce la latenza da O(N) 
 *   a O(log N).
 * - Unità MAC (Multiply-Accumulate): Il cuore dei DSP e delle CPU moderne. La moltiplicazione
 *   è una serie di somme parziali; il CSA permette di sommare queste righe in parallelo.
 * - Algoritmi Crittografici (es. SHA-256): Dove la somma modulare è l'operazione base,
 *   il CSA permette di calcolare round intermedi ultra-veloci.
 * =====================================================================================
 */


unsigned int carry_save_add3(unsigned int a, unsigned int b, unsigned int c, unsigned int *carry_out)
{
    // Calcola la somma isolata dei 3 ingressi (1 se il numero di bit accesi è dispari)
    unsigned int sum = a ^ b ^ c;
    
    // Calcola i riporti (1 se la maggioranza dei bit è accesa, cioè 2 o 3).
    // Lo shift a sinistra (<< 1) allinea geometricamente il riporto alla colonna 
    // successiva di peso superiore.
    *carry_out = ((a & b) | (b & c) | (a & c)) << 1;
    
    // Ritorna la somma parziale (il carry è scritto via puntatore)
    // Le due righe prodotte (sum e *carry_out) sono pronte per essere date in 
    // pasto a un altro livello dell'albero o all'addizionatore finale.
    return sum;
}
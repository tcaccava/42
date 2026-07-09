#include <stdio.h>

/* ========================================================================== */
/* GRAY_DISTANCE_TOTAL.C - CALCOLO DELLA DISTANZA DI HAMMING SU CODICE GRAY  */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/* 1. BACKGROUND TEORICO: LA GEOMETRIA DELL'IPERCUBO                         */
/* */
/* La funzione gray_distance calcola quanti bit cambiano fisicamente tra i    */
/* codici Gray di due numeri interi (Distanza di Hamming nello spazio Gray).  */
/* */
/* Se immaginiamo i codici a N bit come i vertici di un ipercubo              */
/* N-dimensionale, il codice Gray rappresenta un percorso hamiltoniano lungo */
/* gli spigoli di questo ipercubo. Ogni passo consecutivo lungo il percorso  */
/* si muove di un solo spigolo, modificando esattamente 1 bit (distanza = 1). */
/* */
/* Quando calcoliamo la distanza tra due stati non consecutivi (es. 3 e 5):  */
/* 1. Convertiamo i valori lineari nello spazio riflesso di Gray.             */
/* 2. Eseguiamo lo XOR tra le due maschere di bit. Lo XOR restituisce un 1    */
/* solo dove i bit sono diversi.                                           */
/* 3. Contiamo il numero di 1 risultanti (operazione di Population Count).     */
/* Il risultato finale indica quante "dimensioni" o spigoli separano i due   */
/* stati all'interno dell'ipercubo logico.                                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 2. APPLICAZIONI PRATICHE NELL'INDUSTRIA                                   */
/* */
/* - DIAGNOSTICA DEI SENSORI (ENCODER): Nelle macchine industriali a controllo*/
/* numerico (CNC), la posizione viene letta in codice Gray. Se il sistema   */
/* rileva che tra due campionamenti successivi la gray_distance e' maggiore */
/* di 1, il firmware genera immediatamente un allarme di "Glitch" o         */
/* "Falso Contatto", poiche' e' fisicamente impossibile saltare due stati   */
/* senza toccare quello intermedio.                                        */
/* */
/* - MODULAZIONI DIGITALI (TELECOMUNICAZIONI): Nelle trasmissioni radio, come */
/* la modulazione QAM (Quadrature Amplitude Modulation) usata nel 4G/5G o   */
/* nei modem Wi-Fi, i bit vengono mappati nello spazio usando il codice     */
/* Gray. Questo fa si' che se il rumore di fondo confonde un segnale con    */
/* quello immediatamente adiacente, la gray_distance sia pari a 1,         */
/* minimizzando il Bit Error Rate (BER) complessivo della trasmissione.    */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 3. IMPLEMENTAZIONE HARDWARE (ALBERO DI WALLACE E PORTE XOR)               */
/* */
/* In un chip ASIC o su una FPGA, questo algoritmo viene eseguito in un       */
/* singolo ciclo di clock senza cicli software, strutturato in tre stadi     */
/* hardware paralleli:                                                       */
/* */
/* STADIO 1: GENERATORI GRAY (Spostamento e XOR)                             */
/* L'input A e l'input B entrano in due blocchi hardware paralleli composti  */
/* da porte XOR a due ingressi. Ogni bit i riceve l'input originale in XOR   */
/* con il bit adiacente (i >> 1). Il ritardo elettrico e' pari a 1 sola porta.*/
/* */
/* STADIO 2: COMPARATORE DI BIT (Matrice XOR centrale)                       */
/* I due vettori Gray (a_gray e b_gray) entrano in un array di 32 porte XOR   */
/* indipendenti lavoranti in parallelo. L'output di questa barriera sara' 1  */
/* solo per le linee elettriche che presentano tensioni discordanti.         */
/* */
/* STADIO 3: ALBERO DI POPCOUNT (Sommatori paralleli / Wallace Tree)         */
/* Per contare i bit a 1 senza fare un ciclo sequenziale, l'hardware usa un  */
/* albero di sommatori combinatori (Full Adders). I bit vengono raggruppati  */
/* a tre a tre, i risultati parziali vengono sommati nei livelli successivi  */
/* riducendo progressivamente i canali fino a ottenere il numero binario     */
/* finale. Nel codice C qui sotto, questo albero hardware viene simulato     */
/* dall'algoritmo SWAR delle maschere esadecimali (0x55555555, 0x33333333...).*/
/* -------------------------------------------------------------------------- */


/* Converte i due ingressi in codice Gray e ne calcola la distanza fisica
*/
unsigned int gray_distance(unsigned int a, unsigned int b)
{
    unsigned int a_gray = a ^ (a >> 1);
    unsigned int b_gray = b ^ (b >> 1);
    return count_diff_bits(a_gray, b_gray);
}

int main(void)
{
    /* Test di validazione hardware: gray_distance(3, 5)
       - 3 in binario e' 011 -> Gray = 011 ^ 001 = 010
       - 5 in binario e' 101 -> Gray = 101 ^ 010 = 111
       - Matrice XOR di confronto: 010 ^ 111 = 101
       - Popcount rileva esattamente due bit accesi a 1.
       
       L'output atteso a terminale e': 2
    */
    printf("%u\n", gray_distance(3, 5));
    
    return 0;
}
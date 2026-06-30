/*
 * =====================================================================================
 * ZIGZAG_SCAN.c - SERIALIZZAZIONE SPAZIALE (JPEG DCT)
 * =====================================================================================
 *
 * ANALISI TEORICA E FONDAMENTI:
 * Il zigzag scan e' un algoritmo di riordinamento spaziale il cui obiettivo è trasformare una matrice 8x8 (dominio spaziale delle frequenze) in
 * un vettore lineare di 64 elementi che preservi l'ordine di importanza dell'informazione.
 *
 * DOMINIO DELLE FREQUENZE E DCT:
 * Dopo la Trasformata Discreta del Coseno (DCT), il blocco JPEG non contiene più pixel, ma
 * coefficienti di frequenza sottoposti a Quantizzazione. La quantizzazione divide i coefficienti per una matrice
 * standard (matrice di quantizzazione) che è più "severa" dove l'occhio umano è meno sensibile,ovvero proprio sulle alte frequenze (in basso a destra).
 * Questo e' il meccanismo di base della compressione lossy di JPEG: la matrice di quantizzazione Q non è uguale per tutti i coefficienti.
 * È una matrice 8 x 8 progettata empiricamente basandosi sulla sensibilità dell'occhio umano.In alto a sinistra (basse frequenze): i valori nella matrice Q
 * sono piccoli: dividendo per un numero piccolo, quindi il coefficiente subisce una perdita di precisione minima. L'occhio umano nota subito se cambiamo le medie
 * o le forme principali dell'immagine. In basso a destra (alte frequenze): i valori nella matrice Q sono molto alti (possono arrivare a 100 o più).
 * Quando dividiamo un coefficiente C per un valore Q grande, il risultato C/Q arrotondato all'intero diventa molto spesso zero.
 * Ecco perché ci si ritrova con un blocco che ha dati significativi in alto a sinistra e una distesa di zeri in basso a destra:
 * - (0,0): Coefficiente DC (frequenza zero), rappresenta la media dell'intensità(luminosita') del blocco.
 * - Resto (AC): Rappresentano le deviazioni dalla media, ovvero i dettagli spaziali.
 * - In alto a sinistra: Basse frequenze (sfumature, transizioni lente).
 * - In basso a destra: Alte frequenze (dettagli fini, bordi netti, rumore).
 * Quando parliamo di "valore" e "media" nella DCT classica (come quella usata nel JPEG originale), stiamo parlando del modello YCbCr, non dell'RGB.
 * Y (Luminanza): È la scala dei grigi. È la "luce" del pixel. È l'unica parte che contiene l'informazione strutturale, i bordi e i dettagli fini.
 * È qui che avviene la magia della DCT e dello Zig-Zag. Cb e Cr (Crominanza): sono i due canali del colore (Blu-Luminanza e Rosso-Luminanza).
 * Gli esseri umani sono molto meno sensibili alle variazioni di colore che alla luminanza. Per questo motivo, questi due canali
 * vengono spesso "sottocampionati" (si buttano via un sacco di dati) prima ancora di passare dalla DCT.
 *
 * TEORIA DELLA QUANTIZZAZIONE: La matrice di quantizzazione agisce come un filtro passa-basso psico-visivo. Dividendo i coefficienti AC ad alta frequenza
 * per valori di quantizzazione elevati, la gran parte di essi viene ridotta a zero. Lo Zig-Zag scan è il complemento necessario a questa operazione:
 * raggruppando questi zeri in fondo al vettore lineare, permette agli algoritmi di compressione successivi (come la codifica Run-Length) di eliminare la ridondanza spaziale,
 * riducendo drasticamente il peso del file. È un sistema perfettamente oliato: la DCT sposta l'informazione, la Quantizzazione elimina quella superflua per l'occhio,
 * e lo Zig-Zag riordina i resti per massimizzare la compressione.
 *
 * LOGICA DEL PERCORSO A "ZIG-ZAG":
 * Il percorso non è casuale ma segue le diagonali della matrice dove la somma degli
 * indici (S = riga + colonna) è costante. Per esempio in una matrice 8 x 8 ogni cella (r,c)
 * appartiene ad una diagonale la cui somma degli indici S = r + c e' costante : Diagonale 0: (0,0) → S=0,Diagonale 1: (0,1), (1,0) → S=1
 * ,Diagonale 2: (2,0), (1,1), (0,2) → S=2...e così via fino alla diagonale 14: (7,7)$ → S=14.
 * - Il movimento avanza per "fronti d'onda" incrementando S ad ogni step.
 * - La direzione di lettura si inverte ad ogni diagonale (alternando tra S pari e dispari)
 * per minimizzare i salti di memoria e mantenere il percorso continuo.
 *
 * PERCHÉ LA LOOKUP TABLE (LUT):
 * Il calcolo in tempo reale delle diagonali (con logica if/else per invertire direzione)
 * sarebbe inefficiente ed estremamente complessa. La LUT,cioe' una tabella di permutazione fissa, è la proiezione lineare pre-calcolata
 * di questo movimento diagonale. Permette un accesso in O(1) e azzera il branching,
 * massimizzando la velocità di esecuzione su hardware moderno.
 *
 * EFFICIENZA DI COMPRESSIONE:
 * La scansione raggruppa le basse frequenze (valori significativi) all'inizio e
 * spinge le alte frequenze (spesso quantizzate a zero) verso la fine. Questo permette
 * alla codifica RLE (Run-Length Encoding) di comprimere drasticamente le lunghe
 * sequenze di zeri finali.
 * =====================================================================================
 */

#include <stdio.h>

// Tabella di permutazione fissa (LUT): traduce le coordinate 2D in indice lineare 1D
// basandosi sul percorso a diagonali con somma costante (S = r + c).
static const unsigned char ZIGZAG_LUT[64] = {
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63};

/**
 * @brief Serializza un blocco 8x8 in un vettore lineare 64-byte.
 * @param block Puntatore al blocco 8x8 (input).
 * @param output Puntatore al buffer di destinazione (output).
 * @return 0 in caso di successo.
 */
unsigned int zigzag_scan(unsigned char *block, unsigned char *output)
{
    for (int i = 0; i < 64; i++)
    {
        // Mappatura lineare tramite LUT per evitare branching logico
        output[i] = block[ZIGZAG_LUT[i]];
    }
    return 0;
}
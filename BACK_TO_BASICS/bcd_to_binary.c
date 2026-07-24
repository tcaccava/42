#include <stdio.h>

/*
 * =====================================================================================
 * BCD_TO_BINARY.c
 * =====================================================================================
 *
 * TEORIA E FONDAMENTI:
 * Il BCD (Binary Coded Decimal) è un formato di codifica dove ogni cifra decimale (0-9)
 * è rappresentata da un nibble (4 bit). In un byte, il nibble alto (bits 7-4)
 * contiene le decine e il nibble basso (bits 3-0) contiene le unità.
 *
 * LOGICA MATEMATICA:
 * Dato un valore BCD:
 * 1. (bcd >> 4): Sposta il nibble alto in posizione 0-3. Questo isola le decine.
 * 2. * 10: Trasforma le decine nel loro valore decimale reale.
 * 3. (bcd & 15): Applica una maschera (0x0F) per isolare le unità nel nibble basso.
 * 4. Somma: Il risultato è il valore binario intero.
 *
 * APPLICAZIONI PRATICHE:
 * Fondamentale nella lettura di dati da sensori (come i chip RTC - Real Time Clock,
 * es. DS1307) che salvano data e ora direttamente in BCD per facilitare il
 * rendering su display a 7 segmenti senza calcoli complessi. Se devi visualizzare
 * l'ora su un display a 7 segmenti, vuoi che il valore "45" minuti sia già pronto
 * come 0x45, così mandi direttamente 0100 e 0101 ai driver dei display senza fare calcoli.
 *
 * Strumentazione Industriale: Voltmetri, contatori di frequenza e pannelli di controllo basati
 * su logica cablata usano il BCD per pilotare i driver dei display LED a 7 segmenti.

 * Calcolatrici Finanziarie: nei calcoli contabili, il BCD è preferito perché evita gli errori
 * di arrotondamento tipici della virgola mobile (float/double).
 * =====================================================================================
 */

unsigned char bcd_to_binary(unsigned char bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 15);
}
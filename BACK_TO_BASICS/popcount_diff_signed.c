#include <stdio.h>
#include "./popcount_swar32.c"

/*
 * =====================================================================================
 * POPULATION_COUNT_DIFFERENCE_SIGNED - ANALISI DELLA DENSITÀ DI BIT
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * La funzione calcola la differenza del "Peso di Hamming" (Hamming Weight) tra due
 * registri a 32-bit. Trattando l'input come bit grezzi (raw bits), l'algoritmo evidenzia
 * l'effetto della rappresentazione in Complemento a Due, dove i numeri negativi hanno
 * sempre il bit di segno (MSB) invertito a 1 e, nel caso di piccoli valori negativi
 * (es. -1 = 0xFFFFFFFF), presentano una densità di bit molto elevata rispetto ai loro
 * corrispondenti positivi.
 *
 * APPLICAZIONI PRATICHE:
 * - Criptoanalisi (Side-Channel Attacks): Nei sistemi embedded, il consumo energetico
 *   di un transistor varia a seconda del numero di bit accesi. La differenza di
 *   popcount permette di modellare il consumo di corrente (Power Analysis) per tentare
 *   di ricostruire chiavi crittografiche tramite analisi statistica del segnale elettrico.
 * - Telecomunicazioni (Line Coding): Utilizzato per monitorare la "disparità" (DC-bias)
 *   nei flussi di dati seriali ad alta velocità. Serve a garantire che il numero di 1
 *   e 0 trasmessi sia bilanciato per evitare distorsioni elettriche sul mezzo fisico.
 *
 * IMPLEMENTAZIONE HARDWARE:
 * Sulle CPU moderne questa operazione è mappata direttamente sull'istruzione nativa
 * 'POPCNT' (implementata nel silicio tramite reti combinatorie di alberi di addizionatori),
 * garantendo l'esecuzione in un singolo ciclo di clock.
 * =====================================================================================
 */

unsigned int popcount_diff_signed(int a, int b)
{
    unsigned int ua = (unsigned int)a;
    unsigned int ub = (unsigned int)b;
    return (popcount_swar32(ua) - popcount_swar32(ub));
}
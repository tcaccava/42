/*
 * =====================================================================================
 * AES_SHIFTROWS - PERMUTAZIONE CICLICA PER LA DIFFUSIONE CRITTOGRAFICA
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * La funzione ShiftRows è uno dei pilastri dell'algoritmo AES (Advanced Encryption
 * Standard - FIPS 197). Il suo scopo è la "diffusione spaziale": mescolare i byte
 * all'interno del blocco di cifratura per evitare che le colonne rimangano isolate
 * nei round successivi. Senza questa fase, la crittanalisi differenziale permetterebbe
 * di spezzare il cifrario in pochi istanti.
 *
 * MATRICE DI STATO (COLUMN-MAJOR ORDER):
 * AES opera su una matrice 4x4 appiattita in un array di 16 byte (0-15).
 * Indici in memoria (colonna per colonna):
 * [0, 4, 8,  12] -> Riga 0
 * [1, 5, 9,  13] -> Riga 1
 * [2, 6, 10, 14] -> Riga 2
 * [3, 7, 11, 15] -> Riga 3
 *
 * LOGICA DI ROTAZIONE (Left Shift):
 * - Riga 0: nessuna rotazione.
 * - Riga 1: rotazione di 1 byte a sinistra.
 * - Riga 2: rotazione di 2 byte a sinistra.
 * - Riga 3: rotazione di 3 byte a sinistra (equivalente a 1 a destra).
 *
 * IMPLEMENTAZIONE HARDWARE/SOFTWARE:
 * In hardware (ASIC/FPGA), non c'è calcolo: è un cablaggio fisico (wire routing).  Prendi
 * il segnale che esce dal registro del byte 1 e lo saldi all'ingresso del registro del
 * byte 13.
 * In software, per massimizzare la velocità, si evita ogni ciclo for a favore di
 * operazioni di swap diretto o permutazioni SIMD, riducendo la latenza al minimo
 * sindacale richiesto per spostare i dati nei registri.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Applica la trasformazione ShiftRows allo stato AES.
 * @param state Puntatore all'array di 16 byte (matrice 4x4).
 */
void aes_shiftrows(unsigned char *state)
{
    unsigned char tmp;

    /* Riga 1: rotazione di 1 byte (1, 5, 9, 13) */
    tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;

    /* Riga 2: rotazione di 2 byte (2, 6, 10, 14) */
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;

    /* Riga 3: rotazione di 3 byte a sinistra (equivalente a 1 a destra: 15 -> 11 -> 7 -> 3 -> 15) */
    tmp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = tmp;
}


int main(void)
{
    /* Inizializzazione matrice 4x4 (numeri 0-15) */
    unsigned char state[16] = {
        0, 4, 8, 12,
        1, 5, 9, 13,
        2, 6, 10, 14,
        3, 7, 11, 15};

    aes_shiftrows(state);

    printf("Stato dopo ShiftRows:\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%2d ", state[i + j * 4]);
        }
        printf("\n");
    }

    return 0;
}
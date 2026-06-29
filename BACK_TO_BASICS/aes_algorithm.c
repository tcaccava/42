/*
 * =====================================================================================
 * AES_MASTER_ALGORITHM - ARCHITETTURA DEL CIFRARIO (AES-128)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * L'AES (Advanced Encryption Standard) non è una serie di funzioni isolate, ma una 
 * "SP-Network" (Substitution-Permutation Network). L'algoritmo esegue 10 round 
 * consecutivi (per chiavi a 128 bit) per trasformare il blocco di testo in chiaro 
 * (16 byte) in testo cifrato.
 *
 * STRUTTURA DEI ROUND:
 * 1. Initial Round: AddRoundKey (Chiave originale).
 * 2. Rounds 1-9: SubBytes -> ShiftRows -> MixColumns -> AddRoundKey.
 * 3. Final Round: SubBytes -> ShiftRows -> AddRoundKey (MixColumns è omesso).
 *
 * PERCHÉ IL FINAL ROUND È DIVERSO?
 * Il MixColumns nel round finale sarebbe matematicamente superfluo: essendo una 
 * trasformazione lineare, se venisse incluso, potrebbe essere invertito senza 
 * conoscere la chiave, riducendo la sicurezza complessiva del sistema.
 *
 * APPLICAZIONI PRATICHE:
 * Standard globale per la protezione dei dati (AES-GCM, TLS, dischi cifrati).
 *
 * =====================================================================================
 */

#include <stdio.h>

// [Includere qui le implementazioni di SubBytes, ShiftRows, MixColumns, AddRoundKey]

/**
 * @brief Esegue un singolo round di AES.
 */
void aes_round(unsigned char *state, unsigned char *round_key, int is_final)
{
    // 1. Confusione
    // aes_sub_bytes(state);
    
    // 2. Permutazione
    // aes_shift_rows(state);
    
    // 3. Diffusione (omessa nel round finale)
    if (!is_final) {
        // aes_mix_columns(state);
    }
    
    // 4. Iniezione Chiave
    aes_add_round_key(state, round_key);
}

/**
 * @brief Algoritmo AES completo: orchestrazione dei 10 round.
 */
void aes_encrypt(unsigned char *state, unsigned char *expanded_key)
{
    // Round iniziale
    aes_add_round_key(state, expanded_key); // Round 0
    
    // Rounds 1-9
    for (int r = 1; r < 10; r++) {
        aes_round(state, expanded_key + (r * 16), 0);
    }
    
    // Round finale (senza MixColumns)
    aes_round(state, expanded_key + (10 * 16), 1);
}

int main(void)
{
    unsigned char state[16] = {0}; // Il tuo blocco di testo da 16 byte
    unsigned char expanded_key[176];
    
    // aes_key_expansion(master_key, expanded_key);
    // aes_encrypt(state, expanded_key);
    
    printf("Cifratura completata.\n");
    return 0;
}
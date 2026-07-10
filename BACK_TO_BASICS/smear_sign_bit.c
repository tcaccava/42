/**
 * =====================================================================================
 * @file        sign_smear_32.c
 * @brief       Propagazione del bit di segno (Sign Smear Mask) per logica branchless.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * -------------------------------------------------------------------------------------
 * 1. BACKGROUND TEORICO: LA LOGICA DELLE MASCHERE ARITMETICHE
 * -------------------------------------------------------------------------------------
 * Nelle architetture che adottano la rappresentazione in Complemento a Due, l'MSB
 * (Most Significant Bit, bit 31 in un intero a 32 bit) determina la natura del numero:
 * - MSB = 0 -> Numero positivo o zero
 * - MSB = 1 -> Numero negativo
 *
 * Lo shift a destra signed (aritmetico) differisce da quello unsigned (logico) perché,
 * per preservare il valore matematico della divisione per due di numeri negativi,
 * replica il bit di segno nei bit vuoti lasciati a sinistra.
 *
 * Eseguendo un'operazione di n >> 31, l'intero registro viene saturato con il valore
 * dell'MSB originario. Matematicamente definiamo la maschera M come:
 *
 * M = n >> 31
 *
 * Generando un output binario discreto:
 * - Se n >= 0, M = 0b0000...0000 = 0x00000000 = 0
 * - Se n < 0,  M = 0b1111...1111 = 0xFFFFFFFF = -1
 *
 * Questa maschera non richiede computazione logica condizionale (if/else), ma crea
 * un vettore di bit utilizzabile direttamente per operazioni booleane di mascheramento.
 *
 * Nota di portabilità: Lo standard C definisce lo shift a destra di un signed negativo
 * come "implementation-defined". Tuttavia, su GCC, Clang, MSVC e su tutte le CPU
 * moderne (x86, ARM, RISC-V), questo viene implementato tassativamente come shift aritmetico.
 *
 * -------------------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE: I PILASTRI DELLA PROGRAMMAZIONE BRANCHLESS
 * -------------------------------------------------------------------------------------
 * Questo pattern è l'atomo fondamentale per eliminare i salti condizionali (if) nelle
 * funzioni matematiche core di motori grafici, DSP (Digital Signal Processing) e codec.
 *
 * A) VALORE ASSOLUTO BRANCHLESS (abs)
 *    Per calcolare |n| senza rami condizionali, sfruttiamo la maschera M:
 *    abs(n) = (n ^ M) - M
 *    - Se n è positivo: M = 0. Quindi (n ^ 0) - 0 = n.
 *    - Se n è negativo: M = -1 (0xFFFFFFFF). Lo XOR con -1 inverte tutti i bit
 *      (equivale a ~n). La sottrazione successiva di -1 equivale a sommare 1.
 *      Per la definizione di complemento a due, ~n + 1 è esattamente -n (positivo).
 *
 * B) CLAMP / MIN / MAX (Limitazione di intervalli)
 *    Se dobbiamo selezionare il minimo tra due numeri senza if, possiamo calcolare
 *    la maschera sul segno della loro differenza (a - b):
 *    M = (a - b) >> 31
 *    min_ab = b + ((a - b) & ~M);
 *
 * C) SATURATING ADDITION (Somma saturata in audio/video)
 *    Nelle manipolazioni audio (PCM), se una somma supera INT_MAX, non deve fare il
 *    wrap-around (che distorcerebbe il suono distruggendo il segnale), ma deve bloccarsi
 *    al valore massimo. Usando maschere di segno combinate tra addendi e risultato, si
 *    isolano i bit di overflow per forzare il registro a 0x7FFFFFFF o 0x80000000
 *    tramite semplici bitwise AND/OR.
 *
 * -------------------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONE HARDWARE: PIPELINE EFFICIENCY
 * -------------------------------------------------------------------------------------
 * Sulle CPU moderne con pipeline profonde ed esecuzione fuori ordine (Out-of-Order),
 * un salto condizionale non predetto correttamente (branch misprediction) causa il
 * flussaggio dell'intera pipeline, costando dai 15 ai 20 cicli di clock vuoti.
 *
 * - Architettura x86_64: Questo codice mappa direttamente sull'istruzione SAR
 *   (Shift Arithmetic Right). sar eax, 31 richiede 1 solo ciclo di clock ed ha una
 *   latenza bassissima.
 * - Architettura ARM: Utilizza l'istruzione ASR (Arithmetic Shift Right). Su ARM la
 *   potenza è doppia: grazie al barrel shifter hardware, l'operazione di shift può spesso
 *   essere fusa "gratis" nello stesso ciclo dell'operazione successiva. Ad esempio, l'abs
 *   branchless si scrive in un'unica istruzione assembly composta:
 *   `EOR R1, R0, R0, ASR #31` seguita da `SUB R1, R1, R0, ASR #31`.
 */

/**
 * @brief Propaga il bit di segno su tutti i 32 bit del registro.
 * @param n Valore intero signed a 32 bit.
 * @return int 0x00000000 se positivo/zero, 0xFFFFFFFF se negativo.
 */
int smear_sign_bit(int n)
{
    return n >> 31;
}

int main(void)
{
    int neg = -42;
    int zero = 0;
    int pos = 1337;

    printf("--- VERIFICA SIGN SMEAR MASK ---\n\n");

    printf("Input: %d\t-> Maschera: 0x%08X (Decimale: %d)\n", neg, smear_sign_bit(neg), smear_sign_bit(neg));
    printf("Input: %d\t-> Maschera: 0x%08X (Decimale: %d)\n", zero, smear_sign_bit(zero), smear_sign_bit(zero));
    printf("Input: %d\t-> Maschera: 0x%08X (Decimale: %d)\n", pos, smear_sign_bit(pos), smear_sign_bit(pos));

    // Dimostrazione pratica: ABS branchless integrato
    int v = -15;
    int mask = smear_sign_bit(v);
    int abs_v = (v ^ mask) - mask;

    printf("\n--- ESEMPIO APPLICATIVO: ABS BRANCHLESS ---\n");
    printf("Valore iniziale: %d\n", v);
    printf("Maschera generata: 0x%08X\n", mask);
    printf("Risultato Abs: %d (Ottenuto senza istruzioni di salto)\n", abs_v);

    return 0;
}
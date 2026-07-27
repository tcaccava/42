/*******************************************************************************
 * PROMOZIONE IMPLICITA DEI TIPI, DATA MODELS (LP64 vs LLP64) E HARDWARE
 *******************************************************************************
 * 
 * 1. PERCHÉ BASTA CASTARE UN SOLO OPERANDO: "USUAL ARITHMETIC CONVERSIONS"
 * 
 * Lo standard C (ISO/IEC 9899) definisce una serie di regole rigide chiamate 
 * "Usual Arithmetic Conversions" (Conversioni Aritmetiche Consuete).
 * 
 * Quando un operatore binario (come la moltiplicazione *) riceve due operandi 
 * di tipo diverso, il compilatore applica la promozione di tipo prima di 
 * eseguire l'operazione hardware:
 * - Nel codice (long long)a * b, a viene convertito esplicitamente a long long.
 * - L'operatore * si trova ora con un operando long long (a 64 bit) e un 
 *   operando int (a 32 bit).
 * - Per lo standard, l'operando con grado inferiore (int) viene convertito 
 *   IMPLICITAMENTE al tipo dell'operando con grado superiore (long long).
 * - Di conseguenza, anche b viene promosso a long long prima del calcolo.
 * - La moltiplicazione viene eseguita nativamente dall'ALU a 64 bit, producendo 
 *   un risultato a 64 bit senza rischio di overflow intermedio.
 * 
 * NOTA SULLA TRAPPOLA COMUNE:
 * Se si scrivesse long long)(a * b), l'operazione a * b verrebbe eseguita 
 * PER PRIMA tra due interi a 32 bit. In caso di overflow, si verificherebbe 
 * un "Signed Integer Overflow" (comportamento indefinito / Undefined Behavior) 
 * che troncherebbe i bit in eccesso PRIMA del cast a 64 bit, rendendo il cast 
 * del tutto inutile.
 * 
 * -----------------------------------------------------------------------------
 * 2. DIFFERENZA TRA long E long long SU PIATTAFORME A 64 BIT
 * 

 * La dimensione in bit dei tipi interi C dipende dal "Data Model" adottato dal 
 * sistema operativo e dall'architettura (ABI).
 * 
 * A) Modello LP64 (Linux, macOS, BSD, iOS, Android su 64 bit):
 *    - `int`:       32 bit
 *    - `long`:      64 bit
 *    - `long long`: 64 bit
 *    - Puntatori:   64 bit
 *    Su questi sistemi, long e long long hanno la stessa identica ampiezza.
 * 
 * B) Modello LLP64 (Windows a 64 bit - x64 / ARM64):
 *    - `int`:       32 bit
 *    - `long`:      32 BIT (Rimasto a 32 bit per retrocompatibilità Win32!)
 *    - `long long`: 64 bit
 *    - Puntatori:   64 bit
 *    Su Windows 64-bit, usare long per evitare l'overflow comporterebbe un 
 *    BUG GRAVISSIMO, perché long rimane a 32 bit e la moltiplicazione andrebbe 
 *    comunque in overflow!
 * 
 * C99 GARANZIA CROSS-PLATFORM:
 * Lo standard C99 garantisce che long long sia SEMPRE largo *almeno* 64 bit 
 * su qualsiasi piattaforma (32-bit, 64-bit, Windows, Linux, embedded). 
 * Pertanto, l'uso di long long (o dell'alias `int64_t` da `<stdint.h>`) è l'unico 
 * modo portabile e sicuro per garantire un accumulatore a 64 bit.
 * 
 * -----------------------------------------------------------------------------
 * 3. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Motori Fisici e DSP senza FPU: Moltiplicazioni in virgola fissa per oggetti 
 *   in movimento, audio processing o filtraggio di segnali digitali in tempo reale.
 * - Matematica Finanziaria: Calcolo di interessi o valute ad alta precisione 
 *   (es. importi memorizzati come interi scalati) per evitare errori di arrotondamento 
 *   dei float (`0.1 + 0.2 != 0.3`).
 * - Algoritmi Crittografici (RSA, ECC): Prodotti intermedi tra grandi interi 
 *   che necessitano di accumulatori a larghezza doppia per evitare perdite di riporto.
 * 
 * -----------------------------------------------------------------------------
 * 4. IMPLEMENTAZIONE ED ESECUZIONE HARDWARE (ASSEMBLY)
 * -----------------------------------------------------------------------------
 * - Su x86_64: 
 *   Il compilatore trasforma il cast ed estende il segno (Sign Extension) 
 *   usando l'istruzione MOVSXD (Move with Sign-Extension Doubleword). 
 *   La moltiplicazione viene poi eseguita tramite l'istruzione a 64 bit `IMUL %rax, %rbx`, 
 *   che scrive il risultato in un registro generale a 64 bit (es. RAX).
 * 
 * - Su ARM64:
 *   Le architetture ARM hanno un'istruzione dedicata pensata esattamente per questo 
 *   pattern: SMULL (Signed Multiply Long). 
 *   `SMULL X0, W1, W2` prende due registri sorgente a 32 bit (W1, W2), li moltiplica 
 *   e deposita direttamente il risultato completo a 64 bit nel registro X0 in un 
 *   singolo ciclo di clock, senza bisogno di istruzioni di estensione separate.
 *******************************************************************************/

#include <stdio.h>

// Moltiplica due numeri in virgola fissa con frac_bits bit frazionari.
// Include arrotondamento al valore più vicino (+0.5) prima dello shift.
int fixed_point_mul(int a, int b, int frac_bits) 
{
    // 1. Cast esplicito di a a (long long): forza b a essere promosso implicitamente 
    //    a 64 bit. Il prodotto viene calcolato a piena precisione 64-bit.
    long long mult = (long long)a * b;

    //  2. Arrotondamento (opzionale ma consigliato): aggiunge 0.5 nel formato fisso
    //     (corrispondente al bit subito a destra del punto frazionario)
    // if (frac_bits > 0)
    // {
    //     mult += (1LL << (frac_bits - 1));
    // }

    // 3. Shift a destra aritmetico (preserva il segno) e ridimensionamento a int
    return (int)(mult >> frac_bits);
}

int main(void) 
{
    // Test: 1.0 (256) * 1.5 (384) con 8 bit frazionari = 1.5 (384)
    int res = fixed_point_mul(256, 384, 8);
    printf("Risultato: %d\n", res);

    return 0;
}
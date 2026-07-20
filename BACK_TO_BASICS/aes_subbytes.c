/* ============================================================================
 * DOCUMENTAZIONE COMPLETA: GENERAZIONE RUNTIME DELLA S-BOX DI AES (SUBBYTES)
 * ============================================================================
 * 1. BACKGROUND TEORICO: L'ALGEBRA DELLA CONFUSIONE (SHANNON)
 * ----------------------------------------------------------------------------
 * La fase SubBytes e' l'unico elemento non lineare di tutto l'algoritmo AES.
 * Senza questa trasformazione, l'intero cifrario si ridurrebbe a una gigantesca
 * equazione lineare risolvibile in pochi millisecondi tramite l'eliminazione 
 * di Gauss. La S-Box e' il pilastro della "Confusione".
 *
 * Gli autori del Rijndael (Joan Daemen e Vincent Rijmen) l'hanno progettata
 * combinando due distinte operazioni algebriche:
 *
 * A) L'Inversione Moltiplicativa in GF(2^8)
 * Ogni byte dello stato viene interpretato come un polinomio di grado massimo 7
 * nel campo finito di Galois GF(2^8). L'operazione cruciale consiste nel trovare
 * il suo inverso moltiplicativo x^-1 tale che:
 *
 * x * x^-1 == 1  (mod P(x))
 *
 * Dove P(x) e' il polinomio irriducibile standard di AES (Rijndael):
 * P(x) = x^8 + x^4 + x^3 + x + 1  (valore esadecimale: 0x11B)
 *
 * Per convenzione crittografica, l'elemento 0x00 non ha inverso e viene mappato
 * direttamente su se stesso (0x00). Questa inversione garantisce la massima
 * non-linearita' possibile e riduce al minimo la correlazione tra input e output,
 * distruggendo l'efficacia della crittanalisi lineare e differenziale.
 *
 * B) La Trasformazione Affine (Barriera anti-algebrica)
 * Se l'algoritmo si fermasse all'inversione moltiplicativa, la S-Box avrebbe una
 * struttura matematica troppo pulita ed esposta ad attacchi basati su sistemi
 * di equazioni polinomiali. 
 * Per rompere questa regolarita' geometrica, il byte invertito (b) subisce una
 * trasformazione affine sopra GF(2). Viene moltiplicato per una matrice binaria
 * fissa A (8x8) e sommato in XOR con una costante C = 0x63 (01100011).
 *
 * Formula bit-a-bit (dove i va da 0 a 7):
 * y[i] = b[i] ^ b[(i+4)%8] ^ b[(i+5)%8] ^ b[(i+6)%8] ^ b[(i+7)%8] ^ c[i]
 *
 * Nel software low-level, questa moltiplicazione di matrici si implementa senza
 * loop lenti combinando il byte con se stesso ruotato a sinistra (ROL) di 1, 
 * 2, 3 e 4 posizioni tramite bitwise XOR, sommandovi infine 0x63.
 * ============================================================================
 */

#include <stdio.h>

// Rotazione circolare a sinistra a 8 bit, type-safe
static inline unsigned char rol8(unsigned char val, unsigned int shift)
{
    return ((val << shift) | (val >> (8 - shift)));
}

// Moltiplicazione branchless nel Campo di Galois GF(2^8) con polinomio 0x1B (0x11B senza MSB)
static inline unsigned char gf_mul(unsigned char a, unsigned char b)
{
    unsigned char p = 0;

    for (int i = 0; i < 8; i++)
    {
        p ^= a & -(b & 1);
        unsigned char mask = -(a >> 7);
        a = (a << 1) ^ (0x1B & mask);
        b >>= 1;
    }
    return (p);
}

// Inversione moltiplicativa via Teorema di Fermat: x^-1 = x^254 in GF(2^8)
static inline unsigned char gf_inv(unsigned char x)
{
    if (x == 0)
        return (0);

    unsigned char res = 1;
    unsigned char base = x;
    unsigned char exp = 254; // 0b11111110

    while (exp > 0)
    {
        if (exp & 1)
            res = gf_mul(res, base);
        base = gf_mul(base, base);
        exp >>= 1;
    }
    return (res);
}

// Applica l'inversione e la trasformazione affine ad un singolo byte
static inline unsigned char aes_sbox_runtime(unsigned char b)
{
    unsigned char inv = gf_inv(b);

    // Trasformazione Affine di AES: y = inv ^ ROL(inv,1) ^ ROL(inv,2) ^ ROL(inv,3) ^ ROL(inv,4) ^ 0x63
    return (inv ^ rol8(inv, 1) ^ rol8(inv, 2) ^ rol8(inv, 3) ^ rol8(inv, 4) ^ 0x63);
}

// Funzione principale richiesta: elabora lo stato a 16 byte in-place
void ft_aes_subbytes(unsigned char *state)
{
    for (int i = 0; i < 16; i++)
    {
        state[i] = aes_sbox_runtime(state[i]);
    }
}

int main(void)
{
    // Stato di test fittizio (matrice 4x4 lineare da 0x00 a 0x0F)
    unsigned char state[16] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F
    };

    printf("Stato ORIGINALE:\n");
    for (int i = 0; i < 16; i++)
        printf("%02X ", state[i]);
    printf("\n\n");

    ft_aes_subbytes(state);

    // Output atteso per i primi elementi: S-Box(0)=63, S-Box(1)=7C, S-Box(2)=77...
    printf("Stato dopo ft_aes_subbytes (Confusione):\n");
    for (int i = 0; i < 16; i++)
        printf("%02X ", state[i]);
    printf("\n");

    return (0);
}

/* ============================================================================
 * 2. ANALISI MECCANICA DEL CODICE SORGENTE (MATEMATICA DEI REGISTRI)
 * ----------------------------------------------------------------------------
 * - gf_mul: Moltiplicazione nel campo. Esegue l'algoritmo di Peasant modificato.
 * Usa maschere bitwise condizionali (`-(b & 1)` e `-(a >> 7)`) per simulare 
 * gli if in modo branchless. Se il bit piu' alto di 'a' e' 1, applica lo XOR
 * con 0x1B (il polinomio di Rijndael ridotto a 8 bit eliminando il nono bit).
 * - gf_inv: Calcola l'inverso sfruttando il Piccolo Teorema di Fermat esteso
 * ai campi finiti: x^(p^n - 1) == 1 => x^255 == 1 => x * x^254 == 1.
 * Quindi x^-1 e' equivalente a elevare x alla 254-esima potenza.
 * L'esponente 254 (11111110 in binario) viene processato tramite l'algoritmo
 * Square-and-Multiply riducendo il calcolo a soli 7 quadrati e 6 moltiplicazioni.
 * ============================================================================
 * 3. APPLICAZIONI PRATICHE: SCENARI DI PRODUZIONE RUNTIME
 * ----------------------------------------------------------------------------
 * Nelle implementazioni embedded standard o su sistemi general-purpose si tende
 * a usare tabelle statiche precalcolate (Look-Up Tables da 256 byte). Tuttavia,
 * calcolarla dinamicamente a runtime e' mandatorio in scenari architetturali specifici:
 *
 * - Architetture Ultra-Constrained (Smart Cards / IoT):
 * In dispositivi embedded estremi dove la memoria ROM/RAM e' misurata in singoli
 * byte, allocare 256 byte per la S-Box e altri 256 byte per la S-Box inversa (RS-Box)
 * e' impraticabile. La generazione a runtime riduce l'occupazione di memoria a zero.
 *
 * - Crittografia White-Box & Meccanismi DRM:
 * Per impedire agli attaccanti di estrarre le chiavi crittografiche tramite analisi
 * statica della memoria o reverse engineering, gli sviluppatori modificano
 * dinamicamente il polinomio irriducibile o la costante affine a runtime. Generando
 * S-Box mutabili al volo si spezzano i signature-scanner dei debugger.
 * ============================================================================
 * 4. IMPLEMENTAZIONE HARDWARE: LOGICA COMBINATORIA SUL SILICIO
 * ----------------------------------------------------------------------------
 * Se si dovesse mappare il loop di Fermat (Square-and-Multiply) su un circuito
 * integrato hardware (FPGA o ASIC), la latenza in cicli di clock sarebbe disastrosa.
 * L'hardware segue strategie diametralmente opposte:
 *
 * - Campi Compositi (Architettura di Canright):
 * L'approccio hardware moderno piu' efficiente decompone il campo spaziale GF(2^8)
 * in campi isomorfi piu' piccoli (campi compositi):
 * GF(2^8) -> GF((2^4)^2) -> GF(((2^2)^2)^2)
 * Invece di calcolare l'inverso di un intero a 8 bit (operazione pesantissima), 
 * il silicio esegue l'inversione su blocchi logici elementari a 2 bit (implementati
 * con semplici porte XOR e AND hardware in cascata asincrona) e poi mappa il
 * risultato verso l'alto. Questo riduce la dimensione fisica del chip dell'80%.
 *
 * - Accelerazione Nativa della CPU (Istruzioni AES-NI):
 * Poiche' le Look-Up Tables software sono vulnerabili ad attacchi di temporizzazione
 * della cache (cache-timing attacks), i produttori di CPU hanno integrato la logica
 * geometrica della S-Box direttamente nel silicio.
 * Le architetture x86_64 implementano le istruzioni hardware dedicate `AESENC` e
 * `AESENCLAST`, mentre ARM espone istruzioni all'interno del modulo Cryptography
 * Extension. Quando la CPU incontra questi opcode, lo stato a 16 byte viene fatto 
 * passare attraverso una rete di transistor combinatoria che applica SubBytes, 
 * ShiftRows e MixColumns in un singolo ciclo di clock, in tempo costante e al
 * riparo da qualsiasi side-channel attack.
 * ============================================================================
 */

 
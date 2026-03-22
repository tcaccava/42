#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main() {

    // =========================================================================
    // RAPPRESENTAZIONI NUMERICHE
    // lo stesso numero puo' essere scritto in basi diverse, il compilatore le
    // converte tutte nello stesso valore binario in memoria
    // =========================================================================

    int dieci     = 10;         // decimale,    base 10, notazione normale
    int dieci_b   = 0b00001010; // binario,     base 2,  notazione 0b
    int dieci_hex = 0x0a;       // esadecimale, base 16, notazione 0x
    int dieci_ott = 012;        // ottale,      base 8,  notazione 0
    printf("%d %d %d %d\n", dieci, dieci_b, dieci_hex, dieci_ott); // 10 10 10 10

    // unsigned char e' il tipo perfetto per un singolo byte:
    // - "unsigned" significa che va da 0 a 255 (nessun numero negativo)
    // - occupa esattamente 8 bit in memoria
    // - puo' rappresentare esattamente 2^8 = 256 valori distinti
    unsigned char nove_bit = 0b00001001; // nove in binario

    // =========================================================================
    // SHIFT A DESTRA >>
    // sposta tutti i bit verso destra di N posizioni
    // i bit che escono a destra vengono persi
    // equivale a dividere per 2^N (divisione intera, la virgola viene scartata)
    // =========================================================================

    printf("%d\n", nove_bit >> 1); // 00001001 → 00000100 = 4  (9/2=4)
    printf("%d\n", nove_bit >> 2); // 00001001 → 00000010 = 2  (9/4=2)

    // =========================================================================
    // SHIFT A SINISTRA <<
    // sposta tutti i bit verso sinistra di N posizioni
    // i bit che escono a sinistra vengono persi, a destra entrano zeri
    // equivale a moltiplicare per 2^N
    // =========================================================================

    printf("%d\n", nove_bit << 1); // 00001001 → 00010010 = 18  (9*2=18)
    printf("%d\n", nove_bit << 2); // 00001001 → 00100100 = 36  (9*4=36)

    // =========================================================================
    // AND &
    // confronta i bit posizione per posizione:
    // 1 & 1 = 1   (entrambi 1 → 1)
    // 1 & 0 = 0   (almeno uno 0 → 0)
    // 0 & 0 = 0
    //
    // usi principali:
    // - ESTRARRE bit specifici (maschera di 1 nella posizione che vuoi leggere)
    // - SPEGNERE bit specifici (maschera di 0 nella posizione che vuoi spegnere)
    // =========================================================================

    unsigned char maschera = 0b00000001;
    unsigned char risultato = nove_bit & maschera; // estrae il bit 0
    // 00001001
    // 00000001
    // --------
    // 00000001 = 1
    printf("%d\n", risultato); // 1

    // spegnere il bit 3 di nove usando AND con una maschera di 0 in posizione 3:
    unsigned char spento = nove_bit & 0b11110111;
    // 00001001
    // 11110111
    // --------
    // 00000001 = 1  (il bit 3 e' spento, gli altri intatti)
    printf("%d\n", spento); // 1
    // oppure piu'elegantemente:

    unsigned char spento2 = nove_bit & ~(1 << 3);
    printf("%d\n", spento2);
    // =========================================================================
    // OR |
    // confronta i bit posizione per posizione:
    // 1 | 1 = 1
    // 1 | 0 = 1
    // 0 | 0 = 0   (da 0 SOLO se entrambi sono 0)
    //
    // uso principale:
    // - ACCENDERE bit specifici (maschera di 1 nella posizione che vuoi accendere)
    // =========================================================================

    unsigned char maschera2 = 0b00000110;
    unsigned char risultato2 = nove_bit | maschera2;
    // 00001001
    // 00000110
    // --------
    // 00001111 = 15
    printf("%d\n", risultato2); // 15

    // uso combinato AND e OR: accendo il bit 2 poi lo spengo
    unsigned char nove_acceso = nove_bit | 0b00000100;  // accendo bit 2
    // 00001001 | 00000100 = 00001101 = 13
    printf("%d\n", nove_acceso); // 13

    nove_acceso = nove_acceso & 0b11111011; // spengo bit 2
    // 00001101 & 11111011 = 00001001 = 9
    printf("%d\n", nove_acceso); // 9, siamo tornati a nove
    //oppure piu' elegantemente : nove_acceso &= ~(1 << 2)

    // =========================================================================
    // XOR ^
    // OR ESCLUSIVO: da 1 quando i due bit sono DIVERSI, 0 quando sono UGUALI
    // 1 ^ 1 = 0   (uguali → 0)
    // 0 ^ 0 = 0   (uguali → 0)
    // 1 ^ 0 = 1   (diversi → 1)
    // 0 ^ 1 = 1   (diversi → 1)
    //
    // usi principali:
    // - INVERTIRE (toggle) bit specifici: XOR con 1 inverte, XOR con 0 lascia intatto
    // - scambiare due variabili senza usare una variabile temporanea
    // - cifratura base (XOR con una chiave)
    // =========================================================================

    printf("%d\n", nove_bit ^ 0b00001111);
    // 00001001
    // 00001111
    // --------
    // 00000110 = 6  (i bit dove la maschera ha 1 vengono invertiti)

    // toggle del bit 3: se era 1 diventa 0, se era 0 diventa 1
    unsigned char toggled = nove_bit ^ 0b00001000;
    // 00001001 ^ 00001000 = 00000001 = 1
    printf("%d\n", toggled); // 1

    // scambio senza variabile temporanea (trucco classico con XOR):
    unsigned char x = 5, y = 9; //00000101  00001001
    x ^= y; // x = x ^ y 00001100
    y ^= x; // y = y ^ (x ^ y) = x originale 00001001
    x ^= y; // x = (x ^ y) ^ x = y originale 00000101
    printf("%d %d\n", x, y); // 9 5, scambiati

    // =========================================================================
    // NOT ~
    // inverte tutti i bit: 0 diventa 1, 1 diventa 0
    //
    // ATTENZIONE: integer promotion
    // quando fai operazioni bitwise, il C promuove automaticamente i tipi
    // piu' piccoli di int (come unsigned char) a int prima dell'operazione.
    // questo vale per tutti gli operatori (&, |, ^, ~, <<, >>).
    // con & | ^ non te ne accorgi perche' i bit alti restano 0.
    // con ~ i bit alti diventano tutti 1 e il problema diventa visibile.
    // =========================================================================

    // dimostrazione della promozione: unsigned char promosso a int prima della somma
    unsigned char a = 200;
    unsigned char b = 100;
    int           res = a + b;        // 300, corretto: int non trabocca
    unsigned char ris = a + b;        // 44:  troncato a 8 bit (300 - 256 = 44)
    printf("%d\n", res); // 300
    printf("%d\n", ris); // 44

    // ~nove_bit: prima promosso a int (32 bit), poi invertito
    // 00000000 00000000 00000000 00001001  (nove come int)
    // 11111111 11111111 11111111 11110110  (dopo ~)
    // il bit piu' alto e' 1 → numero negativo
    // per leggere il valore: inverti → 00001001 = 9, aggiungi 1 → 10, metti - → -10

    int           intero    = ~nove_bit;            // vede tutti i 32 bit → -10
    unsigned char carattere = ~nove_bit;            // tronca a 8 bit → 11110110 = 246
    uint64_t      grande    = (uint64_t)~(uint64_t)nove_bit; // ~su 64 bit → numero enorme

    printf("%d\n",  intero);    // -10
    printf("%d\n",  carattere); // 246
    printf("%lu\n", grande);    // 18446744073709551606

    // uso pratico di ~: spegnere un bit senza scrivere la maschera a mano
    // invece di calcolare a mano la maschera inversa, la generi con ~(1 << i)
    unsigned char n = 0b11111111;
    n = n & ~(1 << 3); // spegne il bit 3
    // 1 << 3       = 00001000
    // ~(1 << 3)    = 11110111
    // n & 11110111 = 11110111 = 247
    printf("%d\n", n); // 247

    // =========================================================================
    // RIEPILOGO: pattern standard per operare su un singolo bit i
    //
    // var |=  (1 << i);   // ACCENDI il bit i   (OR  con maschera di 1)
    // var &= ~(1 << i);   // SPEGNI  il bit i   (AND con maschera di 0, generata con ~)
    // var ^=  (1 << i);   // INVERTI il bit i   (XOR con maschera di 1)
    // (var >> i) & 1      // LEGGI   il bit i   (shift + AND)
    // =========================================================================

    // --- ESEMPIO 1: permessi file stile Unix --------------------------------
    // ogni bit rappresenta un permesso on/off
    // questo e' esattamente come funziona chmod() nel kernel Linux
    #define PERMESSO_LETTURA    (1 << 0) // bit 0 = 00000001
    #define PERMESSO_SCRITTURA  (1 << 1) // bit 1 = 00000010
    #define PERMESSO_ESECUZIONE (1 << 2) // bit 2 = 00000100

    unsigned char permessi = 0; // nessun permesso

    permessi |= PERMESSO_LETTURA;    // accendo lettura   → 00000001
    permessi |= PERMESSO_SCRITTURA;  // accendo scrittura → 00000011

    // controllo se il permesso di lettura e' attivo
    if ((permessi >> 0) & 1)
        printf("lettura: si\n");     // stampa: si

    // controllo se l'esecuzione e' attiva
    if (!((permessi >> 2) & 1))
        printf("esecuzione: no\n");  // stampa: no

    permessi &= ~PERMESSO_SCRITTURA; // spengo scrittura  → 00000001
    printf("permessi finali: %d\n", permessi); // 1

    // --- ESEMPIO 2: stato di 8 LED ------------------------------------------
    // immagina di controllare 8 LED con un singolo byte
    // ogni bit = un LED (1 acceso, 0 spento)
    // questo e' esattamente come si scrive in un registro hardware su Arduino
    // o su qualsiasi microcontrollore

    unsigned char led = 0b00000000; // tutti spenti

    led |=  (1 << 0); // accendo LED 0  → 00000001
    led |=  (1 << 7); // accendo LED 7  → 10000001
    led ^=  (1 << 0); // inverto LED 0  → 10000000 (era acceso, ora spento)
    led &= ~(1 << 7); // spengo  LED 7  → 00000000

    printf("stato led: %d\n", led); // 0, tutti spenti

    // --- ESEMPIO 3: leggere i bit uno per uno ------------------------------
    // dato un numero qualsiasi, stampa "bit N: 0/1" per ogni bit
    unsigned char valore = 0b10110100;
    for (int i = 7; i >= 0; i--)
        printf("bit %d: %d\n", i, (valore >> i) & 1);
    // stampa:
    // bit 7: 1
    // bit 6: 0
    // bit 5: 1
    // bit 4: 1
    // bit 3: 0
    // bit 2: 1
    // bit 1: 0
    // bit 0: 0

    // --- ESEMPIO 4: contare quanti bit sono accesi (popcount) --------------
    // utile in crittografia, correzione errori, algoritmi su grafi
    unsigned char val = 0b10110101; // ha 5 bit accesi
    int contatore = 0;
    for (int i = 0; i < 8; i++)
        contatore += (val >> i) & 1; // somma 1 per ogni bit acceso
    printf("bit accesi: %d\n", contatore); // 5

    // =========================================================================
    // SHIFT SU SIGNED VS UNSIGNED — comportamento diverso a destra
    //
    // shift a sinistra << : uguale per entrambi (moltiplica per 2^N)
    // shift a destra  >> : dipende dal tipo
    //   - unsigned: riempie sempre con 0 a sinistra (logical shift)
    //   - signed:   replica il bit del segno (arithmetic shift)
    //               se il numero e' positivo entra 0, se negativo entra 1
    // Perché entra 1 a sinistra con lo shift su negativi?
    // Perché il processore ha due istruzioni di shift diverse:
    //
    // SHR — logical shift right, entra sempre 0 (usata per unsigned)
    // SAR — arithmetic shift right, replica il bit del segno (usata per signed)
    //
    // Il compilatore C sceglie SAR quando il tipo è int. L'idea è che SAR preserva il segno — shiftare un negativo a destra deve dare un negativo più piccolo (divisione per 2), e se entrasse 0 il bit del segno cambierebbe.
    //     // questo e' uno dei bug piu' subdoli del C: se usi >> su un int negativo
    //     // il risultato non e' una semplice divisione per 2
    // =========================================================================

    unsigned int u = 0b10000000000000000000000000000001u;
    int          s = (int)0b10000000000000000000000000000001u; // stesso pattern, signed

    printf("%u\n", u >> 1); // entra 0 a sinistra → 01000000... = 1073741824
    printf("%d\n", s >> 1); // entra 1 a sinistra → 11000000... = -1073741824

    // regola pratica: nelle operazioni bitwise usa SEMPRE tipi unsigned
    // int, long ecc. possono darti risultati inaspettati con >> e ~

    // =========================================================================
    // HEX E BINARIO — corrispondenza diretta
    //
    // ogni cifra esadecimale corrisponde esattamente a 4 bit (un nibble)
    // e' il motivo per cui il codice a basso livello usa quasi sempre hex:
    // - 0xFF  = 11111111  (tutti i bit a 1)
    // - 0x0F  = 00001111  (nibble basso tutto a 1)
    // - 0xF0  = 11110000  (nibble alto tutto a 1)
    // - 0xAA  = 10101010  (bit alternati)
    // - 0x55  = 01010101  (bit alternati, inverso)
    //
    // tabella completa:
    // 0=0000  1=0001  2=0010  3=0011
    // 4=0100  5=0101  6=0110  7=0111
    // 8=1000  9=1001  A=1010  B=1011
    // C=1100  D=1101  E=1110  F=1111
    //
    // per leggere 0xB3: B=1011, 3=0011 → 10110011
    // per scrivere 11010110 in hex: 1101=D, 0110=6 → 0xD6
    // =========================================================================

    // estrarre il nibble alto e basso di un byte
    unsigned char byte_val   = 0xB3;                          // 10110011
    unsigned char nibble_alto  = (byte_val >> 4) & 0x0F;     // 1011 = 0xB
    unsigned char nibble_basso =  byte_val       & 0x0F;     // 0011 = 0x3
    printf("nibble alto: %X, basso: %X\n", nibble_alto, nibble_basso); // B, 3

    // ricostruire il byte dai due nibble
    unsigned char ricostruito = (nibble_alto << 4) | nibble_basso;
    printf("ricostruito: %X\n", ricostruito); // B3

    // =========================================================================
    // TRUCCHI ARITMETICI CON AND
    // =========================================================================

    // --- pari o dispari ---
    // un numero e' dispari se e solo se il bit 0 e' acceso
    // molto piu' veloce di n % 2
    int num = 42;
    if (num & 1)
        printf("dispari\n");
    else
        printf("pari\n"); // stampa: pari

    // --- potenza di 2 ---
    // una potenza di 2 ha esattamente un solo bit acceso: 0001, 0010, 0100...
    // sottraendo 1 si ottiene una maschera di tutti 1 sotto quel bit
    // il AND tra i due e' 0 se e solo se era una potenza di 2
    // ATTENZIONE: funziona solo per n > 0
    int p = 64;
    if (p > 0 && (p & (p - 1)) == 0)
        printf("%d e' una potenza di 2\n", p); // stampa: 64 e' una potenza di 2

    // --- isolare il bit piu' basso acceso ---
    // n & (-n) isola il bit piu' basso acceso, azzera tutti gli altri
    // perche funziona: -n in complemento a due e' ~n + 1,
    // il +1 genera un carry che si propaga fino al primo bit acceso,
    // che e' l'unico che coincide tra n e -n
    int k = 0b10110100;
    int lowest = k & (-k); // isola il bit 2 → 0b00000100 = 4
    printf("bit piu' basso acceso: %d\n", lowest); // 4

    // iterare su tutti i bit accesi uno alla volta:
    int bits = 0b10110100;
    while (bits) {
        int bit = bits & (-bits); // isola il bit piu' basso
        printf("bit acceso: %d\n", bit);
        bits &= bits - 1;         // spegni il bit piu' basso e vai al prossimo
    }
    // stampa: 4, 32, 128

    // =========================================================================
    // ROTAZIONI
    // lo shift normale perde i bit che escono — la rotazione li rimette
    // dall'altro lato. C non ha un operatore dedicato, ma questo pattern
    // viene riconosciuto dai compilatori e compilato in una singola
    // istruzione CPU (ROL/ROR su x86)
    // =========================================================================

    uint32_t r = 0x80000001u; // 10000000 00000000 00000000 00000001
    int rot = 1;

    uint32_t rotl = (r << rot) | (r >> (32 - rot)); // ruota a sinistra
    uint32_t rotr = (r >> rot) | (r << (32 - rot)); // ruota a destra

    printf("originale: %08X\n", r);    // 80000001
    printf("rot left:  %08X\n", rotl); // 00000003
    printf("rot right: %08X\n", rotr); // C0000000

    // le rotazioni sono il mattone base della crittografia:
    // SHA-256, AES, ChaCha20 sono costruiti quasi interamente su XOR e rotazioni

    // =========================================================================
    // BRANCHLESS OPERATIONS — operazioni senza if
    // nei percorsi critici (inner loop, codice real-time, kernel) i branch
    // (if/else) sono costosi perche' rompono la pipeline del processore.
    // con le operazioni bitwise puoi fare le stesse cose senza salti condizionali
    // =========================================================================

    // --- valore assoluto senza if ---
    // se n >= 0: mask = 0x00000000, XOR lascia intatto, sottrai 0
    // se n <  0: mask = 0xFFFFFFFF, XOR inverte tutti i bit, sottrai -1
    //            che e' esattamente ~n + 1 = -n
    int neg  = -42;
    int mask = neg >> 31;              // arithmetic shift: propaga il bit del segno
    int abs_val = (neg ^ mask) - mask;
    printf("abs(-42) = %d\n", abs_val); // 42

    // --- minimo e massimo senza if ---
    int aa = 10, bb = 20;
    int minimo  = bb ^ ((aa ^ bb) & -(aa < bb));
    int massimo = aa ^ ((aa ^ bb) & -(aa < bb));
    printf("min: %d, max: %d\n", minimo, massimo); // 10, 20

    // =========================================================================
    // SWAR — SIMD Within A Register
    // operare su piu' byte in parallelo dentro un singolo uint64_t senza loop.
    // la stessa tecnica usata in printbits.
    // fondamentale per riscrivere strlen, memchr, memcmp in modo efficiente:
    // invece di leggere un byte alla volta, leggi 8 byte e cerchi il risultato
    // in parallelo su tutti e 8
    // =========================================================================

    // --- rilevare un byte zero dentro un uint64_t (base di strlen veloce) ---
    // strlen veloce legge 8 byte alla volta e cerca il byte 0 (terminatore)
    uint64_t word = 0x48656c6c6f000000ULL; // "Hello\0\0\0" in memoria

    // sottrai 1 da ogni byte: se era 0x00 diventa 0xFF e genera borrow nel byte sopra
    // AND con ~word: tiene solo i bit dove word aveva 0 (elimina falsi positivi)
    // AND con 0x8080...: guarda solo il bit 7 di ogni byte
    // se un byte di zero_mask e' non-zero, il byte corrispondente in word era 0
    uint64_t zero_mask = (word - 0x0101010101010101ULL) & ~word & 0x8080808080808080ULL;
    printf("byte zero trovato: %s\n", zero_mask ? "si" : "no"); // si

    // --- cercare un byte specifico (base di memchr veloce) ---
    // per cercare 'e' (0x65): XOR con 0x6565... trasforma 'e' in 0x00,
    // poi applica il test per byte zero
    uint64_t target = 0x6565656565656565ULL;
    uint64_t xored  = word ^ target;
    uint64_t found  = (xored - 0x0101010101010101ULL) & ~xored & 0x8080808080808080ULL;
    printf("byte 'e' trovato: %s\n", found ? "si" : "no"); // si

    // =========================================================================
    // ENDIANNESS E BYTE SWAPPING
    // su x86 la RAM e' little-endian: il byte meno significativo sta
    // all'indirizzo piu' basso. su reti (TCP/IP) e molti formati file
    // si usa big-endian. convertire tra i due e' una delle operazioni
    // piu' comuni quando si scrive codice di sistema o di rete
    // =========================================================================

    uint32_t big_endian = 0x01020304;

    uint32_t swapped =
        ((big_endian & 0x000000FF) << 24) | // byte 0 → posizione 3
        ((big_endian & 0x0000FF00) <<  8) | // byte 1 → posizione 2
        ((big_endian & 0x00FF0000) >>  8) | // byte 2 → posizione 1
        ((big_endian & 0xFF000000) >> 24);   // byte 3 → posizione 0

    printf("big endian: %08X\n", big_endian); // 01020304
    printf("swapped:    %08X\n", swapped);    // 04030201

    // in produzione si usa __builtin_bswap32/__builtin_bswap64
    // che compila in una singola istruzione CPU, ma il principio e' questo

    // =========================================================================
    // OVERFLOW DETECTION — rilevare overflow senza undefined behavior
    // in C l'overflow su int signed e' undefined behavior: non puoi fare
    // a + b e controllare dopo. devi rilevarlo prima con operazioni bitwise.
    // l'overflow di somma avviene quando i due operandi hanno lo stesso segno
    // e il risultato ha segno diverso
    // =========================================================================

    int x2 = 2147483647; // INT_MAX
    int y2 = 1;

    // estrai il bit del segno con >> 31 (0 se positivo, -1 se negativo)
    // stesso segno tra x2 e y2: (x2 ^ y2) >> 31 == 0
    // segno diverso tra x2 e risultato: (x2 ^ (x2+y2)) >> 31 != 0
    if (!((x2 ^ y2) >> 31) && ((x2 ^ (x2 + y2)) >> 31))
        printf("overflow rilevato!\n"); // stampa: overflow rilevato!

    return 0;
}

// =============================================================================
// RIFERIMENTO: BIT TRICKS — pattern standard del bit hacking
// questi non si inventano, si studiano e si riconoscono.
// fonte: Hacker's Delight (Henry Warren) — il libro di riferimento del settore
// =============================================================================
 
// --- OPERAZIONI SU SINGOLO BIT i ---------------------------------------------
 
// leggi il bit i
// (n >> i) & 1
 
// accendi il bit i
// n |= (1 << i)
 
// spegni il bit i
// n &= ~(1 << i)
 
// inverti il bit i
// n ^= (1 << i)
 
// --- OPERAZIONI SUL BIT PIU' BASSO ACCESO ------------------------------------
 
// isola il bit piu' basso acceso          10110100 → 00000100
// n & (-n)
 
// spegni il bit piu' basso acceso         10110100 → 10110000
// n & (n - 1)
 
// accendi tutti i bit sotto il piu' basso 10110100 → 10110111
// n | (n - 1)
 
// isola la sequenza di 0 in fondo         10110100 → 00000011
// ~n & (n - 1)
 
// --- OPERAZIONI SUL BIT PIU' ALTO ACCESO -------------------------------------
 
// isola il bit piu' alto acceso           00101000 → 00100000
// n |= n >> 1;
// n |= n >> 2;
// n |= n >> 4;   // per uint16 aggiungi >> 8, per uint32 anche >> 16
// n - (n >> 1)
 
// --- TEST --------------------------------------------------------------------
 
// e' zero?
// n == 0
 
// e' pari?
// !(n & 1)
 
// e' dispari?
// n & 1
 
// e' potenza di 2?  (solo per n > 0)
// !(n & (n - 1))
 
// hanno lo stesso segno?
// !((x ^ y) >> 31)
 
// --- ARITMETICA --------------------------------------------------------------
 
// valore assoluto senza if
// mask = n >> 31
// (n ^ mask) - mask
 
// minimo senza if
// b ^ ((a ^ b) & -(a < b))
 
// massimo senza if
// a ^ ((a ^ b) & -(a < b))
 
// moltiplicazione per 2^k
// n << k
 
// divisione per 2^k  (solo unsigned o positivi)
// n >> k
 
// modulo potenza di 2  (equivale a n % (2^k), solo per n >= 0)
// n & ((1 << k) - 1)
 
// --- ROTAZIONI ---------------------------------------------------------------
 
// ruota a sinistra di k su N bit
// (n << k) | (n >> (N - k))
 
// ruota a destra di k su N bit
// (n >> k) | (n << (N - k))
 
// --- SWAP --------------------------------------------------------------------
 
// scambia due variabili senza variabile temporanea
// a ^= b
// b ^= a
// a ^= b
 
// scambia i due nibble di un byte
// (n << 4) | (n >> 4)
 
// inverti i byte di un uint32 (big/little endian)
// __builtin_bswap32(n)
 
// inverti i byte di un uint64
// __builtin_bswap64(n)
 
// --- CONTEGGIO ---------------------------------------------------------------
 
// conta i bit accesi (popcount) — algoritmo di Kernighan
// int c = 0;
// while (n) { n &= n - 1; c++; }
 
// conta i bit accesi — versione parallela SWAR su byte
// n = n - ((n >> 1) & 0x55)
// n = (n & 0x33) + ((n >> 2) & 0x33)
// n = (n + (n >> 4)) & 0x0F
 
// parita' (1 se numero dispari di bit accesi, 0 altrimenti)
// n ^= n >> 4
// n ^= n >> 2
// n ^= n >> 1
// n & 1
 
// --- SWAR — costanti fondamentali su uint64_t --------------------------------
 
// copia un byte in tutti e 8 i byte
// byte * 0x0101010101010101ULL
 
// guarda solo il bit alto di ogni byte
// 0x8080808080808080ULL
 
// sottrai 1 da ogni byte in parallelo
// - 0x0101010101010101ULL
 
// trova byte zero in una word (base di strlen veloce)
// (word - 0x0101010101010101ULL) & ~word & 0x8080808080808080ULL
 
// trova byte specifico c in una word (base di memchr veloce)
// word ^= c * 0x0101010101010101ULL   // trasforma c in 0
// poi applica il test per byte zero
 
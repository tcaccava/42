#include <stdio.h>

// Il PEXT (Parallel Bits Extract) a 32 bit in modalità SWAR (SIMD Within A Register) è uno dei mostri sacri della programmazione bitwise.
// A 8 bit, te la puoi cavare con una manciata di passaggi fissi perché lo spazio di manovra è ridotto. A 32 bit, un bit che si trova in posizione 31 potrebbe dover scivolare fino alla posizione 0.
// Se provassimo a calcolare lo shift esatto per ogni singolo bit, genereremmo un'esplosione di codice. La soluzione SWAR, invece, risolve il problema in appena 5 passi(fasi),
// sfruttando le potenze di due (2^0, 2^1, 2^2, 2^3, 2^4).
// Invece di chiedersi "Di quanto devo shiftare questo bit?", l'algoritmo si chiede:
// 1)"Questo blocco deve saltare a destra di 1 posizione?"
// 2) "Deve saltare di 2?"
// 3) "Deve saltare di 4?"
// 4) "Deve saltare di 8?"
// 5) "Deve saltare di 16?"
// Combinando questi 5 salti potenziali, un bit può spostarsi di una distanza qualsiasi compresa tra 0 e 31 posizioni.
// Ad ogni stage(1, 2, 4, 8, 16),l'algoritmo esegue una magia algebrica basata sul calcolo dei buchi (i bit a zero della maschera).
// Prendiamo lo stage in cui verifichiamo i salti di 4 posizioni :
// 1)Contare i vuoti : l'algoritmo analizza la maschera corrente e genera una "maschera di shift". Questa maschera temporanea conterrà degli 1 solo in corrispondenza dei bit che hanno almeno 4 zeri alla loro destra nella maschera reale.
// 2)Spostamento condizionale branchless : sfruttando il trucco del meno unario o dello shifting mascherato l'algoritmo seleziona solo i bit di n che si trovano sopra questi "buchi" e li shifta a destra di 4 in un colpo solo.
// I bit che non avevano abbastanza vuoti sotto di sé rimangono fermi.
// 3)Aggiornamento della Geometria : la maschera subisce lo stesso identico trattamento.Gli 1 della maschera che si sono spostati vengono shiftati a destra di 4. In questo modo, al prossimo stage,
// la maschera rifletterà la nuova posizione compressa dei bit.
// La Magia Matematica del "Sotto-passaggio"
// Per evitare i cicli la generazione della maschera di shift ad ogni stage usa una proprietà dei numeri binari chiamata propagazione del prefisso(Prefix Sum) .
// Se tu hai una maschera e vuoi sapere dove sono i vuoti, non li conti. Usi operazioni come mask = mask | (mask << 1), poi mask | (mask << 2), e così via.
// Questo espande gli 1 della maschera verso sinistra, riempiendo i vuoti e creando dei blocchi solidi.Facendo lo XOR o l'and logico tra la maschera originale e questa versione "espansa",
// i confini dei blocchi ti dicono esattamente quali bit devono saltare e di quanto, senza che tu debba fare alcuna struttura di controllo.
// Ricapitolando l'architettura dei 32 bit:
// Fase 1(passo 1) : comprime i bit vicini eliminando i singoli zeri isolati.I bit si spostano al massimo di 1 posizione.La maschera viene aggiornata.
// Fase 2(passo 2) : elimina i blocchi di due zeri.I bit saltano di 2 posizioni.La maschera viene aggiornata.
// Fase 3(passo 4) : elimina i blocchi di quattro zeri.I bit saltano di 4 posizioni.La maschera viene aggiornata.
// Fase 4(passo 8) : spostamenti massicci di 8 posizioni.
// Fase 5(passo 16) : ultimo grande spostamento di 16 posizioni.
// Al termine della quinta fase, tutti i "buchi"(gli zeri della maschera originaria)sono stati matematicamente spinti a sinistra del registro, e tutti i bit validi si ritrovano stipati all'estrema destra, perfettamente contigui.
// È un approccio che a prima vista sembra controintuitivo perché elabori anche i bit che non ti interessano,
// ma la CPU esegue queste operazioni parallele ad una velocità spaventosa rispetto a un ciclo `while` che interroga 32 bit uno per uno.

// AMBITI DI UTILIZZO DEL PEXT:
// 1) Crittografia e Cifrari: Manipolazione ultra-rapida di blocchi di dati, permutazioni di bit nei cifrari a blocchi (come DES/AES o cifrari leggeri per IoT).
// 2) Motori di Scacchi: Estrazione rapida dei bit dalle Bitboard per calcolare le mosse possibili dei pezzi scorrevoli (Torre, Alfiere, Regina) usando le tabelle "Magic Bitboards".
// 3) Database e Compressione: Compattazione di indici bitmap, deserializzazione rapida di formati dati compressi basati su colonne (es. Parquet, ORC) e algoritmi come FastPFOR.
// 4) Elaborazione Segnali e Grafica: Mascheramento e impacchettamento di canali colore o coordinate pixel sparse.

// NOTA SULL'IMPLEMENTAZIONE HARDWARE:
// Data la complessità computazionale di questo approccio SWAR in software (comunque molto più veloce di un ciclo), Intel e AMD hanno implementato l'istruzione nativa hardware PEXT in silicio
// a partire dalle architetture Haswell (Intel, 2013) e Zen 3 (AMD, 2020) nel set di istruzioni BMI2. In hardware, l'operazione viene completata in pochissimi cicli di clock (spesso 1-3 cicli)
// grazie a una rete di commutazione interna (Benes o Banyan Network) che bypassa totalmente tutta l'algebra software qui sotto.

unsigned int bit_compress(unsigned int n, unsigned int mask)
{
    unsigned int res = 0;
    // Iteriamo bit per bit solo sulle posizioni attive della maschera
    for (unsigned int sub_mask = 1; mask != 0; sub_mask <<= 1)
    {
        // Isola il bit più a destra attualmente attivo nella maschera usando il trick del complemento a due
        unsigned int low_bit = mask & -mask;

        // Se in quel punto n ha il bit a 1, lo iniettiamo nella posizione corrente del risultato
        res |= (sub_mask) & -!!(n & low_bit); // n & low_bit restituisce un numero, usando il doppio!! convertiamo qualsiasi valore diverso da zero in 1,e lo zero in 0: in questo modo se il bit e' acceso la maschera sara 00xFFFFFFFF e quindi iniettiamo 1 in res,altrimenti 0 e inietteremo 0

        // Spegniamo il bit appena elaborato per far scorrere la maschera al prossimo ciclo
        mask ^= low_bit;
    }
    return res;
}

// l'enormemente complessa e verbosa versione bitwise pura
// unsigned int bit_compress(unsigned int n, unsigned int mask)
// {
//     unsigned int mk, mp, mv, t;
//     // Solo i bit selezionati dalla maschera originale partecipano al processo
//     n &= mask;

//     // Fase 1: Spostamento potenziale di 1 posizione (potenza 2^0)
//     mk = ~mask << 1;
//     mp = mk | (mk << 1);
//     mp = mp | (mp << 2);
//     mp = mp | (mp << 4);
//     mp = mp | (mp << 8);
//     mp = mp | (mp << 16);
//     mv = mp & mask;
//     t = n & mv;
//     n = (n ^ t) | (t >> 1);
//     mask = (mask ^ mv) | (mv >> 1);

//     // Fase 2: Spostamento potenziale di 2 posizioni (potenza 2^1)
//     mk = ~mask << 2;
//     mp = mk | (mk << 1);
//     mp = mp | (mp << 2);
//     mp = mp | (mp << 4);
//     mp = mp | (mp << 8);
//     mp = mp | (mp << 16);
//     mv = mp & mask;
//     t = n & mv;
//     n = (n ^ t) | (t >> 2);
//     mask = (mask ^ mv) | (mv >> 2);

//     // Fase 3: Spostamento potenziale di 4 posizioni (potenza 2^2)
//     mk = ~mask << 4;
//     mp = mk | (mk << 1);
//     mp = mp | (mp << 2);
//     mp = mp | (mp << 4);
//     mp = mp | (mp << 8);
//     mp = mp | (mp << 16);
//     mv = mp & mask;
//     t = n & mv;
//     n = (n ^ t) | (t >> 4);
//     mask = (mask ^ mv) | (mv >> 4);

//     // Fase 4: Spostamento potenziale di 8 posizioni (potenza 2^3)
//     mk = ~mask << 8;
//     mp = mk | (mk << 1);
//     mp = mp | (mp << 2);
//     mp = mp | (mp << 4);
//     mp = mp | (mp << 8);
//     mp = mp | (mp << 16);
//     mv = mp & mask;
//     t = n & mv;
//     n = (n ^ t) | (t >> 8);
//     mask = (mask ^ mv) | (mv >> 8);

//     // Fase 5: Spostamento potenziale di 16 posizioni (potenza 2^4)
//     mk = ~mask << 16;
//     mp = mk | (mk << 1);
//     mp = mp | (mp << 2);
//     mp = mp | (mp << 4);
//     mp = mp | (mp << 8);
//     mp = mp | (mp << 16);
//     mv = mp & mask;
//     t = n & mv;
//     n = (n ^ t) | (t >> 16);

//     return n;
// }

int main()
{
    // Esempio pratico: n = 1011 (11 in decimale), mask = 1001 (9 in decimale)
    // Bit estratti nelle posizioni della maschera: il bit 3 (1) e il bit 0 (1).
    // Risultato atteso compattato a destra: 0011 (3 in decimale).
    unsigned int n = 0xB;    // 1011 in binario
    unsigned int mask = 0x9; // 1001 in binario

    printf("Input: 0x%X, Maschera: 0x%X\n", n, mask);
    printf("Risultato Compresso: %u\n", bit_compress(n, mask));

    // Esempio a 32 bit più complesso
    unsigned int n2 = 0xAAAAAAA5;    // 10101010101010101010101010100101
    unsigned int mask2 = 0x000F000F; // Isola solo i 4 bit bassi e 4 bit intermedi
    printf("Risultato Esempio 32bit: %u\n", bit_compress(n2, mask2));// 165

    return 0;
}
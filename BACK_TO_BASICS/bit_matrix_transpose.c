#include <stdio.h>
// per trasposizione di una matrice di bit si intende che ogni riga i di una matrice di unsigned char
// diventa la corrispondente colonna i nella matrice trasposta
// Usata in compressori di immagini e codec video per processare blocchi 8×8 (JPEG usa blocchi 8×8 esattamente per questo motivo).

void bit_matrix_transpose(unsigned char matrix[8])
{
    unsigned char transposed[8] = {0};
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
            transposed[7 - j] |= ((1 << (7 - i)) & -((matrix[i] >> (7 - j)) & 1));
    }
    for (int i = 0; i <= 7; i++)
    {
        matrix[i] = transposed[i];
        printf("%u\n", matrix[i]);
    }
}
// oppure una complessissima versione SWAR pura,ultra efficiente
// void ft_bit_matrix_transpose(unsigned char matrix[8]) {
//     // 1. Carichiamo 64 bit in un colpo solo
//     uint64_t x = *(uint64_t *)matrix;
//     uint64_t t;

//     // 2. Passo 1: Scambio blocchi 4x4 (distanza 28 bit)
//     // Maschera: 0x00000000F0F0F0F0
//     t = (x ^ (x >> 28)) & 0x00000000F0F0F0F0ULL;
//     x = x ^ t ^ (t << 28);

//     // 3. Passo 2: Scambio blocchi 2x2 (distanza 14 bit)
//     // Maschera: 0x0000CCCC0000CCCC
//     t = (x ^ (x >> 14)) & 0x0000CCCC0000CCCCULL;
//     x = x ^ t ^ (t << 14);

//     // 4. Passo 3: Scambio blocchi 1x1 (distanza 7 bit)
//     // Maschera: 0x00AA00AA00AA00AA
//     t = (x ^ (x >> 7)) & 0x00AA00AA00AA00AAULL;
//     x = x ^ t ^ (t << 7);

//     // 5. Riscarichiamo i 64 bit nella matrice originale
//     *(uint64_t *)matrix = x;
// }
// Immaginiamo una matrice 8x8 caricata nel registro a 64 bit, tutti i bit B e R sono 1
// (matrix[0])  B B B B  R R R R
// (matrix[1])  B B B B  R R R R
// (matrix[2])  B B B B  R R R R
// (matrix[3])  B B B B  R R R R
// (matrix[4])  0 0 0 0  0 0 0 0
// (matrix[5])  0 0 0 0  0 0 0 0
// (matrix[6])  0 0 0 0  0 0 0 0
// (matrix[7])  0 0 0 0  0 0 0 0

// Primo passo: scambiamo il quadrante superiore destro (AD) con l'inferiore sinistro (BS).
// shiftiamo x di 28 posizioni a destra, che equivale a shiftarla di 4 righe in giu'(>> 32) e poi di 4 righe a sx(<< 4)
// xoriamo x con se stesso shiftato di 28 posizioni per isolare solo i bit differenti,e poi applichiamo una maschera 
// 0x00000000F0F0F0F0ULL,che equivale ad una maschera con i nibble alti accesi nelle ultime 4 righe,per isolare specificamente
// le differenze logiche nel quadrante inferiore sinistro,che salviamo in t.
// L'operazione x = x ^ t ^ (t << 28) applica lo XOR swap:
// 1. x ^ t: il Delta applicato a BS lo trasforma in AD (i due BS si annullano).
// 2. x ^ (t << 28): il Delta riportato sopra trasforma AD in BS (i due AD si annullano).
// I quadranti sulla diagonale (AS e BD) restano invariati perché lì t è 0.



// Situazione dopo il Passo 1 (Scambio 4x4):
// B B B B  0 0 0 0
// B B B B  0 0 0 0
// B B B B  0 0 0 0
// B B B B  0 0 0 0
// R R R R  0 0 0 0
// R R R R  0 0 0 0
// R R R R  0 0 0 0
// R R R R  0 0 0 0

// Secondo passo: entriamo nei blocchi 4x4 e scambiamo i quadratini 2x2 non diagonali.
// La distanza è 14 bit (2 righe giù, 2 colonne a sx: 16 - 2 = 14).
// La maschera 0x0000CCCC0000CCCC isola il Delta per questi blocchi medi.
// Questo "rimescola" i bit B e R portandoli verso le loro colonne finali.



// Situazione dopo il Passo 2 (Scambio 2x2):
// B B  0 0  0 0  0 0
// B B  0 0  0 0  0 0
// B B  B B  0 0  0 0   <-- I bit iniziano a "verticalizzarsi"
// B B  B B  0 0  0 0
// R R  0 0  0 0  0 0
// R R  0 0  0 0  0 0
// R R  R R  0 0  0 0
// R R  R R  0 0  0 0

// Terzo passo: scambio finale dei singoli bit (1x1).
// Distanza 7 bit (1 riga giù, 1 colonna a sx: 8 - 1 = 7).
// Maschera "a scacchiera" 0x00AA00AA00AA00AA. Ogni coppia adiacente viene trasposta.

// Risultato finale: la matrice è completamente trasposta.
// Quello che era una riga lunga (8 bit su 4 righe) è diventata una colonna alta (4 bit su 8 righe).
// B B B B  0 0 0 0
// B B B B  0 0 0 0
// B B B B  0 0 0 0
// B B B B  0 0 0 0
// R R R R  0 0 0 0
// R R R R  0 0 0 0
// R R R R  0 0 0 0
// R R R R  0 0 0 0

int main()
{
    unsigned char matrix[8] = {255, 0, 0, 0, 0, 0, 0, 0};
    bit_matrix_transpose(matrix);
}

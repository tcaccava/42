#include <stdio.h>
// Scambia il bit in posizione i con quello in posizione j dentro n.
unsigned int bit_mirror_swap(unsigned int n, int i, int j)
{
    i &= 31; // edge case se i e j sono superiori a 31,evita undefined behavior
    j &= 31;
    unsigned int ibit = (n >> i) & 1; // estraggo i bit in posizione i e j
    unsigned int jbit = (n >> j) & 1;
    return n ^ (((1 << i) ^ (1 << j)) & -(ibit ^ jbit)); // se i bit sono uguali ritorno n,altrimenti n con i bit in posizione i e j invertiti
}
// Esiste un approccio ancor piu' geniale riportato in Hacker's delight,che usa solo due shift e nessuna negazione matematica,quindi ancor piu'
// efficiente del gia' efficientissimo approccio di sopra
// unsigned int bit_mirror_swap_sota(unsigned int n, int i, int j)
// {
//     i &= 31;
//     j &= 31;

//     // x sarà 1 se i bit in posizione i e j sono diversi, 0 se sono uguali
//     unsigned int x = ((n >> i) ^ (n >> j)) & 1;

//     // Se x è 1, questa operazione inverte sia il bit i che il bit j.
//     // Se x è 0, lo XOR con 0 lascia tutto immodificato.
//     return n ^ (x << i) ^ (x << j);
// }

// Implementazione Hardware: le istruzioni PEXT e PDEP (Intel/AMD BMI2)
// Nelle CPU moderne con architettura x86 (da Haswell in poi), gli ingegneri si sono resi conto che fare lo swap o la permutazione di bit sparsi usando shift e XOR software era troppo lento.
// Invece di creare un'istruzione per scambiare solo due bit (troppo specifica), hanno scolpito nel silicio due istruzioni hardware mostruose:
// -PEXT (Parallel Extract): Prende i bit da un registro in base a una maschera e li impacchetta tutti di fila a destra.
// -PDEP (Parallel Deposit): Prende i bit impacchettati e li ridistribuisce in posizioni arbitrarie basandosi su una maschera.
// Combinando una PEXT e una PDEP, la CPU può eseguire il bit_mirror_swap (e permutazioni molto più complesse di decine di bit contemporaneamente) in un solo ciclo di clock.

// Crittografia: le P-Box (Permutation Boxes)
// Cifrari storici come il DES (Data Encryption Standard) o algoritmi più moderni implementano un layer di sicurezza basato su due concetti: confusione (sostituire bit) e diffusione (rimescolare la posizione dei bit).
// Il rimescolamento avviene nelle P-Box hardware. All'interno di un chip dedicato alla crittografia, i fili elettrici in uscita da un registro sono fisicamente incrociati per fare in modo che il bit alla posizione i vada alla posizione j.
// Il codice branchless serve per simulare via software, nel modo più veloce e sicuro possibile, quel comportamento che i chip crittografici fanno via hardware con i fili di rame incrociati.

// Elaborazione dei Segnali: il Bit-Reversal nella FFT (Fast Fourier Transform)
// La FFT è l'algoritmo alla base di qualsiasi cosa manipoli frequenze: compressione MP3, JPEG, elaborazione di immagini mediche (TAC/Risonanze) e radar.
// Per come è strutturata la matematica della FFT (l'algoritmo di Cooley-Tukey), prima di elaborare i dati, l'array di input deve essere riordinato secondo il bit-reversal ordering.
// Significa che il dato all'indice 001 (1) va scambiato con quello all'indice 100 (4). Di fatto, si fa un mirror-swap dei bit dell'indice: il bit più alto va al posto del bit più basso, il secondo con il penultimo, e così via.

// Nelle CPU ARM esiste un'istruzione hardware dedicata chiamata RBIT che inverte completamente l'ordine dei bit di un registro in un colpo solo, proprio per accelerare la FFT.
// In sintesi: lo swap di due singoli bit isolati è una sottomatrice di un problema hardware più grande (la permutazione parallela).

int main()
{
    printf("%#X %#X\n", bit_mirror_swap(0x80000000, 31, 0), bit_mirror_swap(0x00000001, 31, 0)); // 0x1 0x80000000
    printf("%#X\n", bit_mirror_swap(0x80000001, 31, 0));                                         // immodificato,0x80000001
}
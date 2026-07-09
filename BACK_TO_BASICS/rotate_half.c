/* ========================================================================== */
/* ROTATE_HALF.C - ROTAZIONE A 16 BIT SU REGISTRO A 32 BIT                    */
/* ========================================================================== */

#include <stdio.h>

/* -------------------------------------------------------------------------- */
/* 1. CRITTOGRAFIA SIMMETRICA: I ROUND DI CIFRATURA E LE RETI DI FEISTEL     */
/* */
/* Nei cifrari a blocchi simmetrici (come DES, Blowfish, Camellia o Simon/Speck), */
/* la sicurezza si basa su due pilastri: Confusione e Diffusione.             */
/* */
/* Molti di questi algoritmi utilizzano una struttura chiamata "Rete di       */
/* Feistel". In una rete di Feistel, il blocco di dati iniziale (es. 64 bit)  */
/* viene spezzato a metà: Metà Sinistra (L) e Metà Destra (R).                */
/* Durante ogni round di cifratura, una metà viene elaborata da una funzione  */
/* complessa e poi iniettata nell'altra metà tramite uno XOR. Alla fine del   */
/* round, le due metà devono essere invertite (L diventa R, R diventa L) per  */
/* fare in modo che nel round successivo i dati appena cifrati proteggano     */
/* l'altra metà del blocco.                                                   */
/* */
/* Quando il cifrario lavora su blocchi interni a 32 bit, la funzione     */
/* rotate_half esegue esattamente questo scambio di metà (le Word da 16 bit)  */
/* in modo istantaneo. Questo garantisce che i bit della metà inferiore       */
/* si propaghino nella metà superiore nel round successivo, accelerando la     */
/* diffusione delle modifiche (effetto valanga).                              */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 2. PROTOCOLLI DI RETE (XDR) E GESTIONE DELL'ENDIANNESS                     */
/* */
/* Lo standard XDR (External Data Representation, RFC 4506), utilizzato in    */
/* protocolli come NFS (Network File System), impone che tutti i dati interi  */
/* viaggino sulla rete in formato Big-Endian (Network Byte Order, ovvero il    */
/* byte più significativo per primo).                                         */
/* */
/* Le moderne CPU consumer (x86_64, ARM in modalità standard) sono invece     */
/* Little-Endian: memorizzano il byte meno significativo all'indirizzo più    */
/* basso. Quando ricevi un intero a 32 bit dalla rete su una macchina x86,    */
/* i byte sono totalmente invertiti.                                          */
/* */
/* Per convertire un intero a 32 bit da Little-Endian a Big-Endian (bswap32), */
/* la tua funzione rotate_half rappresenta la seconda metà esatta della       */
/* pipeline di conversione. Una conversione completa a 32 bit si fa così:     */
/* */
/* 1. Si invertono i singoli byte adiacenti dentro le due metà a 16 bit.      */
/* 2. Si usa rotate_half per scambiare le due metà superiori e inferiori.     */
/* */
/* Senza questa rotazione delle metà a 16 bit, i byte rimarrebbero confinati  */
/* nella parte sbagliata del registro, corrompendo il valore del dato XDR.    */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 3. IMPLEMENTAZIONE HARDWARE (IL TRUCCO DEL SILICIO)                        */
/* */
/* Se guardi il codice C, vedi tre operazioni: uno shift a sinistra, uno      */
/* shift a destra e un OR logico. Verrebbe da pensare che la CPU impieghi  */
/* 3 cicli di clock per farlo. In realtà impiega MENO di un ciclo.            */
/* */
/* Nelle CPU moderne (e nelle FPGA/ASIC), questa operazione viene intercettata */
/* dal compilatore e mappata su una singola istruzione hardware nativa di     */
/* rotazione del registro:                                                    */
/* - Su x86: viene tradotta nell'istruzione ROR (Rotate Right) o ROL.         */
/* - Su ARM: viene tradotta nell'istruzione ROR.                              */
/* */
/* A livello di transistor e circuiti logici (Barrel Shifter), la rotazione  */
/* fissa di 16 bit su un registro a 32 bit è praticamente "gratis". In un     */
/* chip dedicato, questa operazione non consuma nemmeno una porta logica: si  */
/* tratta semplicemente di incrociare fisicamente i fili di rame del bus dati */
/* (le piste elettriche dei bit 0-15 vengono collegate direttamente alle     */
/* uscite 16-31, e viceversa). Il ritardo hardware è pari a zero nanosecondi. */
/* -------------------------------------------------------------------------- */

unsigned int rotate_half(unsigned int n) {
    return (n << 16) | (n >> 16);
}

int main(void) {
    /* Test di validazione:
       Input:  0xABCD1234
       Atteso: 0x1234ABCD
    */
    printf("Input originale:  0xABCD1234\n");
    printf("Dopo rotazione:   %#X\n", rotate_half(0xABCD1234));
    
    return 0;
}
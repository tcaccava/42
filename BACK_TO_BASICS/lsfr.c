#include <stdio.h>
//Linear Feedback Shift register,usato in crittografia(cifrari a flusso) e generatori hardware di numeri pseudocasuali,ma anche CRC(cyclic redundancy check),cioe' algoritmi per controlli errori 
//in rete e storage, e spread spectrum per comunicazioni wireless
//implementa un registro a scorrimento con feedback lineare a 8 bit : a ogni passo shifta di 1 a destra 
// e inserisce come bit alto lo XOR dei bit in posizione 7 5 4 3. I bit di feedback sono chiamati tap
// la scelta del tap determina la lunghezza del ciclo: i tap 7 5 4 3 producono un ciclo massimo di 255 valori 
//prima di ripetersi.
unsigned char lfsr(unsigned char n) {
    return (n >> 1) | ((((n & 128) >> 7) ^ ((n & 32) >> 5) ^ ((n & 16) >> 4) ^ ((n & 8) >> 3)) << 7);
}

int main() {
    printf("%d\n", lfsr(0b10110001));//11011000,cioe'216
}
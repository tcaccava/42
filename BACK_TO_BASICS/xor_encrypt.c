#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
// La cifratura XOR è la base di tutti i cifrari moderni. Prendi ogni byte del messaggio e fai XOR con un byte della chiave — il risultato è incomprensibile senza la chiave.
// La chiave si ripete ciclicamente se è più corta del messaggio.
// La proprietà magica: (messaggio ^ chiave) ^ chiave = messaggio — XOR è la sua stessa inversa.
// Quindi encrypt e decrypt sono la stessa identica funzione.

void xor_encrypt(unsigned char *data, unsigned char *key, size_t data_len, size_t key_len)
{
    if (!key_len)
        return;
    size_t i = 0;
    while (i < data_len)
    {
        data[i] ^= key[i % key_len];
        i++;
    }
}
// la funzione che fa decrypt di data encriptato e' encrypt stessa,per le proprietà di reversibilità di XOR
//  void xor_decrypt(unsigned char *data, unsigned char *key, size_t data_len, size_t key_len) {
//      xor_encrypt(data, key, data_len, key_len);
//  }
int main()
{
    unsigned char data[] = "ANNUIT COEPTIS";
    unsigned char key[] = "CIAO";
    xor_encrypt(data, key, 15, 4);
    for (int i = 0; i < 15; i++)
    {
        printf("%u", data[i]);
        printf(" ");
    }
    printf("\n");
}
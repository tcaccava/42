#include <stdio.h>
// È identico a CRC-8 ma su 32 bit, con un polinomio diverso e una particolarità:
// si processa da LSB a MSB invece che da MSB a LSB — per questo il polinomio è "reflected"
//  (i bit sono specchiati rispetto alla versione canonica 0x04C11DB7).
//  Usato in tutti i protocolli di rete (Ethernet, WiFi, USB, ZIP, PNG) e filesystem. È uno degli algoritmi più implementati al mondo.
// calcola il CRC-32 con polinomio 0xEDB88320 — la versione "reflected" usata in Ethernet e zlib.
unsigned int crc32_step(unsigned char data, unsigned int crc)
{
    crc ^= data; // mischia il byte con i 8 bit bassi del crc
    for (int i = 0; i < 8; i++)
    {
        if (crc & 1) // bit basso acceso → feedback
            crc = (crc >> 1) ^ 0xEDB88320;
        else
            crc >>= 1;
    }
    return crc;
}

unsigned int crc32(unsigned char *data, int len)
{
    unsigned int crc = 0xFFFFFFFF;
    for (int i = 0; i < len; i++)
        crc = crc32_step(data[i], crc);
    return crc ^ 0xFFFFFFFF;
}

int main() {
    unsigned char msg[] = "Hello";
    unsigned int checksum = crc32(msg, 5);
    printf("CRC-32 di 'Hello': 0x%08X\n", checksum);  // 0xF7D18982

    // simula errore
    msg[1] ^= 0x01;
    unsigned int checksum_corrotto = crc32(msg, 5);
    printf("CRC-32 corrotto:   0x%08X\n", checksum_corrotto);
    
    if (checksum != checksum_corrotto)
        printf("Errore rilevato!\n");
}
// Il motivo della versione reflected è storico :
// l'hardware seriale degli anni 80 trasmetteva i bit dal LSB, quindi era più efficiente processarli in quell'ordine.
// Lo standard è rimasto anche quando non era più necessario.
// CRC-32 produce un checksum a 32 bit — 4 byte invece di 1.
// Con CRC-8 hai 256 valori possibili — alta probabilità di collisione. Due messaggi diversi hanno 1/256 di probabilità di avere lo stesso CRC per caso.
// Con CRC-32 hai 4 miliardi di valori possibili — probabilità di collisione 1/4294967296.
// Per Ethernet che trasmette miliardi di frame al giorno, CRC-8 sarebbe inutile — avresti falsi positivi continuamente. CRC-32 è abbastanza robusto da essere usato in produzione.
// Il registro deve essere a 32 bit perché il polinomio è a 32 bit — ogni shift e XOR opera su tutti e 32 i bit contemporaneamente.
// Su 8 bit perderesti i bit alti del polinomio e il CRC sarebbe sbagliato.

// 0xEDB88320 in binario è:
// 11101101101110001000001100100000
// Come prima — ogni bit acceso è un termine, l'indice del bit è l'esponente di x. L'x^32 è implicito come l'x^8 nel CRC-8.
// Questo polinomio specifico è stato scelto negli anni 60 da ricercatori IBM perché ha proprietà matematiche che garantiscono:

// rileva tutti gli errori su 1, 2, e 3 bit
// rileva tutti gli errori su burst fino a 32 bit
// rileva il 99.9999998% degli errori più lunghi

// Non è deducibile — è il risultato di ricerca matematica.
// Lo trovi in tutti i protocolli moderni perché è diventato standard de facto.
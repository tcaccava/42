#include <stdio.h>
// calcola il CRC-8 con polinomio 0x07 (x^8 + x^2 + x + 1).
// Il CRC (Cyclic Redundancy Check) è un checksum — prendi un blocco di dati,
// ci applichi una serie di XOR e shift, e ottieni un byte che "riassume" i dati.
// Se anche un solo bit cambia durante la trasmissione, il CRC cambia.
// usato in protocolli di comunicazione seriale (UART, SPI, I2C) e storage (hard disk, SSD)
// per rilevare errori di trasmissione. Il ricevitore ricalcola il CRC e confronta — se non coincide c'è stato un errore.
// Il polinomio 0x07 determina quali bit fanno feedback, esattamente come i tap dell'LFSR:
// 0x07 = 00000111 — rappresenta il polinomio x^2 + x + 1 in binario dove ogni bit è un coefficiente.
// Il bit 2 è acceso → x^2, bit 1 → x, bit 0 → 1.Si chiama CRC-8 perché lavora su 8 bit, e il polinomio completo è x^8 + x^2 + x + 1;
// l'x^8 è implicito, non serve rappresentarlo perché il registro è già a 8 bit.
// Scenario: stai mandando il byte 0x31 via seriale. Potrebbe corrompersi. Come fa il ricevitore a sapere se è arrivato integro?
// Prima di mandarlo, calcoli un checksum — un byte extra che dipende dai dati. Lo mandi insieme al dato. Il ricevitore ricalcola il checksum e confronta — se coincide, il dato è integro.

// data — il byte che stai processando, 0x31 nel nostro esempio.
// crc — il checksum che stai costruendo. Parte da 0, e per ogni byte del messaggio lo aggiorni.
unsigned char crc8(unsigned char data, unsigned char crc)
{
    crc ^= data; // mischia il dato con il CRC attuale
    for (int i = 0; i < 8; i++)
    {
        if (crc & 0x80) // bit alto acceso → overflow
            crc = (crc << 1) ^ 0x07;
        else
            crc <<= 1;
    }
    return crc;
}

int main()
{
    // calcola CRC su una stringa
    unsigned char data[] = "Hello";
    unsigned char crc = 0;
    for (int i = 0; data[i]; i++)
        crc = crc8(data[i], crc);
    printf("CRC di 'Hello': 0x%02X\n", crc);

    // verifica: il ricevitore riceve gli stessi dati + il crc
    // ricalcola e confronta
    unsigned char crc_check = 0;
    for (int i = 0; data[i]; i++)
        crc_check = crc8(data[i], crc_check);

    if (crc_check == crc)
        printf("Dati integri\n");
    else
        printf("Errore rilevato\n");

    // simula un errore — corrompi un byte
    data[1] ^= 0x01; // cambia un bit
    unsigned char crc_corrupted = 0;
    for (int i = 0; data[i]; i++)
        crc_corrupted = crc8(data[i], crc_corrupted);

    if (crc_corrupted == crc)
        printf("Dati integri\n");
    else
        printf("Errore rilevato!\n"); // deve stampare questo
}
// Il valore finale dipende da tutti i byte in sequenza — cambia anche l'ordine dei byte e il CRC cambia. 
// Questo è il punto — un singolo byte riassume l'intero messaggio in modo che qualsiasi modifica lo cambi.
// Il limite è che è solo 8 bit — 256 valori possibili. 
// Quindi teoricamente due messaggi diversi possono avere lo stesso CRC (collisione). 
// Per questo i protocolli seri usano CRC-16 o CRC-32 — più bit, meno collisioni. 
// Il kernel Linux usa CRC-32 per i filesystem, Ethernet usa CRC-32 per i frame di rete.

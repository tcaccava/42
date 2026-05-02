#include <stdio.h>
// una bitmap e' una mappa di bit,cioè una struttura dati,che utilizza i singoli bit per rappresentare informazioni binarie,e che possono ?
// essere manipolati usando l'algebra di Boole
// questa funzione accende il bit all'indice pos della bitmap
void bitmap_set(unsigned char *bitmap, int pos)
{
    bitmap[pos >> 3] |= 1 << (pos & 7);
}
// spegne il bit all'indice pos della bitmap
void bitmap_clear(unsigned char *bitmap, int pos)
{
    bitmap[pos >> 3] &= ~(1 << (pos & 7));
}

int bitmap_test(unsigned char *bitmap, int pos)
{
    return (bitmap[pos >> 3] >> (pos & 7)) & 1;
}
// Contesti di Utilizzo Reali(Perché sono fondamentali)
//     A.Filesystem(Ext4, NTFS, APFS)
//     Quando salvi un file sul PC,
//     il sistema operativo deve sapere quali "blocchi" del disco sono liberi.
//     Invece di leggere l'intero disco, legge la Block Bitmap.
//     Ogni bit a 0 è un blocco libero.
//     Appena scrivi un file,
//     il kernel chiama bitmap_set su migliaia di bit.Senza questo, il tuo SSD sarebbe incredibilmente lento nella gestione dello spazio.

//     B.Allocatori di Memoria(malloc, slab allocator)
//     Quando il kernel gestisce la RAM,la divide in "pagine"(solitamente da 4KB).
//     Una bitmap è il modo più veloce per lo Scheduler o il Memory Manager per trovare il primo "buco" libero nella RAM dove caricare un nuovo programma.

//     C.Networking e Packet Filtering
//     Nei firewall(come iptables o nftables), le bitmap sono usate per gestire set di porte o indirizzi IP.
//     Controllare se una porta è "aperta" o "chiusa" con bitmap_test richiede un tempo costante(O(1)), rendendo il filtraggio dei pacchetti velocissimo anche sotto attacco DDoS.

//     D.Database(Bitmap Indexes)
//     I database(come Oracle o PostgreSQL) usano gli indici bitmap per colonne con "bassa cardinalità"(es.una colonna "Genere" o "Stato Ordine")
//     Fare una query come "Tutti gli uomini che hanno pagato l'ordine" diventa un semplice final_bitmap = gender_bitmap & status_bitmap,
//     un'operazione che la CPU esegue in pochi cicli di clock.
int main()
{
    unsigned char bitmap[] = {0b01001101, 0b01111000}; // 77 e 120: il bit 7 di 120,cioè il bit 15 della bitmap,e' spento
    bitmap_set(bitmap, 15);                            // lo accendiamo
    printf("%u %u\n", bitmap[0], bitmap[1]);           // 77 248,cioe' 120 + 128
    bitmap_clear(bitmap, 15);                          // lo spegniamo
    printf("%u %u\n", bitmap[0], bitmap[1]);           // di nuovo 77 e 120
    printf("%d %d\n", bitmap_test(bitmap, 6), bitmap_test(bitmap, 9));// 1 0
}
#include <stdio.h>
#include <stdint.h>
// Calcola il blending lineare tra due valori: a * (255 - alpha) + b * alpha normalizzato su 255. Senza divisione floating point.
// Usato in GPU e shader per operazioni su colori RGBA: in grafica ogni pixel ha un valore da 0 a 255.
// Se vuoi un colore a metà tra nero (0) e bianco (255) con alpha = 128 (50%), il risultato è grigio (128).
// La formula a * (255 - alpha) + b * alpha è semplicemente una media pesata:
//  alpha = 0 → 100% di a, 0% di b
//  alpha = 255 → 0% di a, 100% di b
//  alpha = 128 → 50% di entrambi
// il problema e'  che a * (255 - alpha) puo' arrivare a 255 * 255 = 65025,ben oltre un unsigned char.Da qui
// la necessita' di usare un uint16_t per il calcolo intermedioe poi dividere per 255. Usando solo le operazioni bitwise
// la divisione pue' essere approssimata con >> 8,che divide per 256: l'errore e'minimo e accettabile in grafica.
unsigned char blend(unsigned char a, unsigned char b, unsigned char alpha){
    return (__uint16_t)((a * (255 - alpha)) + b * alpha) >> 8;
}
// in alterntiva versione piu' precisa,interamente bitwise ma che divide per 255
//  unsigned char blend(unsigned char a, unsigned char b, unsigned char alpha) {
//      uint16_t t = a * (255 - alpha) + b * alpha + 128;
//      return (t + (t >> 8)) >> 8;
//  }
// il punto di partenza e' che 1/255 ≈ 1/256 + 1/65536,quindi:
// t/255 ≈ t/256 + t/65536,cioe' = t >> 8 + t >> 16,ma t >> 16 e' difficile da calcolare su 16 bit
//  il trick sta nell'osservare che t/255 ≈ (t + t/256)/ 256,cioe' ≈ (t + (t >> 8)) >> 8
// il +128 prima e' solo per arrotondare invece di troncare,sposta il punto di taglio a meta' invece che in basso,
// riducendo l'errore massimo da 1 a 0
// in sostanza invece di dividere direttamente per 255,si aggiunge una correzione proporzionale a t stesso
// che compensa l'errore di usare 256 invece di 255

int main()
{
    printf("%d\n", blend(128, 255, 128)); // 191
    printf("%d\n", blend(255, 0, 0));     // 255
    printf("%d\n", blend(0, 128, 255));   // 128
    printf("%d\n", blend(20, 80, 128));   // 50
}

#include <stdio.h>
//usato in grafica o computer vision per binarizzare immagini in grayscale,ogni pixel diventa bianco
//o nero in base ad una soglia
//ritorna 0xFF se n superiore o uguale a t,altrimenti 0
unsigned char threshold(unsigned char n, unsigned char t) {
    return -(n >= t);
}

int main() {
    printf("%d\n", threshold(128, 129));
    printf("%d\n", threshold(128, 128));
    printf("%d\n", threshold(129, 128));

}
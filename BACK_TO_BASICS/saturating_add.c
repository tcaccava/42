#include <stdio.h>
// somma a e b ma se il risultato supera 255 ritorna 255 invece di andare in overflow
unsigned char saturating_add(unsigned char a, unsigned char b)
{
    unsigned char sum = a + b;
    return sum | -(sum < a);
}
//una caratteristica della somma di due byte e' che se non c'e overflow il risultato e' maggiore o uguale
//di entrambi gli addendi,mentre se c'e overflow necessariamente minore di entrambi
//quindi se sum < a e' vera,cioe' 1,l'or di sum con -1,cioe' 255, da 255,mentre se la condizione
//e' falsa,quindi 0, -0 fa zero,e l'or di sum con 0 lascia sum immodificato

int main()
{
    printf("%d\n", saturating_add(100, 100));
    printf("%d\n", saturating_add(200, 100));
    printf("%d\n", saturating_add(255, 1));
}
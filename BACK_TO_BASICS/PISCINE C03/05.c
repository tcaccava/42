#include <string.h>
#include <stdio.h>

// strlcat concatena src alla fine di dest scrivendo al massimo size - 1 byte
// totali in dest e null-terminando sempre la stringa se size > 0.
// La concatenazione avviene solo se strlen(dest) < size; altrimenti non
// viene scritto nulla.
// Ritorna strlen(dest_iniziale) + strlen(src), cioè la lunghezza che la
// stringa finale avrebbe avuto senza limiti di spazio.
// Se il valore di ritorno è >= size, allora è avvenuto un troncamento.

unsigned int ft_strlcat(char *dest, char *src, unsigned int size)
{
    int dlen = 0;
    int slen = 0;
    while (dest[dlen])
        dlen++;
    while (src[slen])
        slen++;
    if (dlen >= size)
        return dlen + slen;
    int i = 0;
    while (src[i] && dlen + i < size - 1)
    {
        dest[dlen + i] = src[i];
        i++;
    }
    dest[dlen + i] = 0;

    return (dlen + slen);
}
int main()
{
    char buf[10] = "ciao";                             // spazio 10
    char *add = "comevaoggi";                          // lunghezza 10
    printf("%d\n", ft_strlcat(buf, add, sizeof(buf))); // printa 14,che e' superiore a size 10,quindi troncamento
    printf("%s\n", buf);                               // ciaocomev
}
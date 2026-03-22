#include <unistd.h>

void ft_swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void ft_putstr(char *s)
{
    while (*s)
        write(1, s++, 1);
    write(1, "\n", 1);
}

/*
** Ordinamento in place della stringa
** Necessario per generare permutazioni in ordine alfabetico
*/
void sort_string(char *s)
{
    int i, j;
    char tmp;

    i = 0;
    while (s[i])
    {
        j = i + 1;
        while (s[j])
        {
            if (s[i] > s[j])
            {
                tmp = s[i];
                s[i] = s[j];
                s[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

/*
** Genera permutazioni in ordine alfabetico
** index = posizione corrente che stiamo fissando
*/
void permute_alpha(char *s, int index)
{
    int i;

    // condizione base: se siamo alla fine della stringa, stampiamo
    if (!s[index])
    {
        ft_putstr(s);
        return;
    }

    i = index;
    while (s[i])
    {
        /*
        ** Evita duplicati nello stesso livello ricorsivo:
        ** se s[i] è uguale a s[index] e non siamo alla posizione iniziale,
        ** saltalo. Questo previene di generare due volte lo stesso prefisso.
        **
        ** funziona SOLO se la stringa è ordinata.
        ** I duplicati sono adiacenti nella stringa sortata, quindi possiamo identificarli così.
        */
        if (i != index && s[i] == s[index])
        {
            i++;
            continue;
        }

        // scelgo il carattere s[i] per la posizione index
        ft_swap(&s[index], &s[i]);

        // ricorsione: fissata s[index], passo alla posizione successiva
        permute_alpha(s, index + 1);

        // backtracking: ripristino lo stato originale della stringa
        ft_swap(&s[index], &s[i]);

        i++;
    }
}

int main(void)
{
    char s[] = "abcda";
    sort_string(s);      // passo obbligatorio: ordino la stringa
    permute_alpha(s, 0); // generazione ricorsiva
    return 0;
}

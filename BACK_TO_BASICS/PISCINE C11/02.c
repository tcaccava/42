
int ft_any(char **tab, int (*f)(char *))
{
    int i = 0;
    while (tab[i])
    {
        if (f(tab[i++]))
            return 1;
    }
    return 0;
}

int has_a(char *s)
{
    while (*s)
    {
        if (*s == 'a')
            return 1;
        s++;
    }
    return 0;
}

int main()
{
    char *tab[] = {"hello", "world", "ciao"};
    ft_any(tab, has_a);//ritorna 1 perche' ciao contiene la a
}
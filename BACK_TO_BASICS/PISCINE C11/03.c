
int ft_count_if(char **tab, int length, int (*f)(char *))
{
    int count = 0;
    int i = 0;
    while (i < length)
    {
        if (f(tab[i++]))
            count++;
    }
    return count;
}

int is_empty(char *s)
{
    return (*s == '\0');
}

int main(void)
{
    char *tab[] = {"", "ciao", "", "hello"};
    int n = ft_count_if(tab, 4, is_empty);
    // n == 2
}

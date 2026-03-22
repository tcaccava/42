#include <stdio.h>

int power(int num, int exp)
{
    if (exp == 0)
        return 1;
    return num * power(num, exp - 1);
}

int is_palindrome(int num)
{
    if(num < 10)//se il numero e' a una cifra e' palindromo per def,caso base
        return 1;
    int tmp = num;//salvo il numero in una temporanea
    int right = num % 10;//isolo l'ultima cifra
    int count = 0;
    while (num > 9)//divido iterativamente per dieci fino ad ottenere una cifra,la prima
    {
        num = num / 10;
        count++;//conto quante volte ho diviso per dieci
    }
    int left = num;//prima cifra
    if (right != left)//se sono diverse blocca
        return 0;
    int middle = (tmp % (10,count)) / 10;//numero composto da cifre rimamenti nel mezzo
    return is_palindrome(middle);//ricorsione su middle
}

int main()
{
    printf("%d\n", is_palindrome(1234));
    printf("%d\n", is_palindrome(12222221));
}
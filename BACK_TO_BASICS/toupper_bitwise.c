#include <stdio.h>
// bitwise branchless
int toupper_bitwise(int c)
{
    return c ^ ((c - 32 ^ c) & -(c >= 97 && c <= 122));
}
// oppure sfruttiamo il fatto che la conversione maiuscolo/minuscolo
// differisce solo di un bit,il bit 5,quindi verifichiamo se il carattere e'minuscolo
//, in tal caso shiftiamo il booleano di 5 unita a sinistra, e con NOT lo invertiamo, cosi
// che l'AND con c spenga quel byte specifico;se invece il carattere non e' minuscolo,
// quindi il bool e' zero,l'AND con NOT zero restituisce direttamente c
//  int ft_toupper(int c) {
//      return c & ~((c >= 'a' && c <= 'z') << 5);
//  }
    int main()
{
    printf("%c %c\n", toupper_bitwise(97), toupper_bitwise(96));
}